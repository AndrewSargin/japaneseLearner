#include "mpv_widget.h"
#include <clocale>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QTimer>
#include <QSGImageNode>
#include <QtQuick/QSGRendererInterface>

void MpvWidget::wakeup(void* ctx) {
    QMetaObject::invokeMethod((MpvWidget*)ctx, "onMpvEvents", Qt::QueuedConnection);
}

void MpvWidget::on_mpv_redraw(void* ctx) {
    MpvWidget* self = (MpvWidget*)ctx;
    self->m_renderThread->requestRender();
}

MpvWidget::MpvWidget(QQuickItem* parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
    setlocale(LC_NUMERIC, "C");

    mpv = mpv_create();
    if (!mpv) qWarning() << "could not create mpv context";

    mpv_set_option_string(mpv, "vo", "libmpv");
    mpv_set_option_string(mpv, "vf", "fps=75");

    if (mpv_initialize(mpv) < 0)
        qWarning() << "could not initialize mpv context";

    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_set_wakeup_callback(mpv, wakeup, this);

    m_surface = new QOffscreenSurface();
    m_renderThread = new MpvRenderThread();

    connect(m_renderThread, &MpvRenderThread::frameRendered,
            this, &MpvWidget::onFrameReady,
            Qt::QueuedConnection);
}

MpvWidget::~MpvWidget() {
    if (m_renderThread) {
        m_renderThread->stop();
        m_renderThread->cleanup();
        delete m_renderThread;
    }
    delete m_surface;
    if (mpv_gl) mpv_render_context_free(mpv_gl);
    mpv_terminate_destroy(mpv);
}

void MpvWidget::onFrameReady() {
    update();
}

QSGNode* MpvWidget::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    QSGImageNode* node = static_cast<QSGImageNode*>(oldNode);
    if (!node)
        node = window()->createImageNode();

    // инициализируем GL и поток один раз
    if (!m_glInitialized) {
        auto get_proc_address = [](void* ctx, const char* name) -> void* {
            Q_UNUSED(ctx)
            QOpenGLContext* glctx = QOpenGLContext::currentContext();
            if (!glctx) return nullptr;
            return reinterpret_cast<void*>(glctx->getProcAddress(QByteArray(name)));
        };

        mpv_opengl_init_params gl_init_params[1] = {get_proc_address, nullptr};
        mpv_render_param params[]{
            {MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL)},
            {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
            {MPV_RENDER_PARAM_INVALID, nullptr}
        };

        if (mpv_render_context_create(&mpv_gl, mpv, params) < 0)
            qWarning() << "failed to initialize mpv GL context";

        m_surface->setFormat(QOpenGLContext::currentContext()->format());
        m_surface->create();

        mpv_render_context_set_update_callback(mpv_gl, on_mpv_redraw, this);

        m_renderThread->prepare(mpv_gl, QOpenGLContext::currentContext(), m_surface);
        m_renderThread->start();
        m_threadStarted = true;
        m_glInitialized = true;
    }

    // обновляем размер в потоке
    m_renderThread->updateSize(QSize(width(), height()));

    // берём текстуру из потока
    GLuint textureId = 0;
    {
        QMutexLocker locker(m_renderThread->mutex());
        auto* fbo = m_renderThread->displayFbo();
        if (fbo && fbo->isValid())
            textureId = fbo->texture();
    }

    if (!textureId) return node;

    // создаём QSGTexture из OpenGL текстуры
    auto* texture = QNativeInterface::QSGOpenGLTexture::fromNative(
        textureId,
        window(),
        QSize(static_cast<int>(width()), static_cast<int>(height()))
    );

    node->setTexture(texture);
    node->setRect(boundingRect());
    node->setOwnsTexture(true);

    return node;
}

void MpvWidget::loadVideo(const std::string& path) {
    QTimer::singleShot(0, this, [this, path]() {
        const char* cmd[] = {"loadfile", path.c_str(), nullptr};
        mpv_command(mpv, cmd);
    });
}

double MpvWidget::getCurrentTime() {
    double time = 0;
    mpv_get_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &time);
    return time;
}

void MpvWidget::onMpvEvents() {
    while (mpv) {
        mpv_event* event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) break;
        handleMpvEvent(event);
    }
}

void MpvWidget::handleMpvEvent(mpv_event* event) {
    switch (event->event_id) {
        case MPV_EVENT_PROPERTY_CHANGE: {
            mpv_event_property* prop = (mpv_event_property*)event->data;
            if (strcmp(prop->name, "time-pos") == 0 && prop->format == MPV_FORMAT_DOUBLE)
                Q_EMIT positionChanged(*(double*)prop->data);
            else if (strcmp(prop->name, "duration") == 0 && prop->format == MPV_FORMAT_DOUBLE)
                Q_EMIT durationChanged(*(double*)prop->data);
            break;
        }
        default: break;
    }
}

void MpvWidget::togglePlayPause() {
    int paused;
    mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &paused);
    paused = !paused;
    mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &paused);
}

void MpvWidget::setVolume(double volume) {
    mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &volume);
}

void MpvWidget::setVideoPosition(double position) {
    mpv_set_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &position);
}
