#include "mpv_render_thread.h"
#include <mpv/render.h>
#include <QOpenGLFunctions>

MpvRenderThread::MpvRenderThread(QObject* parent) : QThread(parent) {}

MpvRenderThread::~MpvRenderThread() {
    stop();
    cleanup();
}

void MpvRenderThread::prepare(mpv_render_context* ctx, QOpenGLContext* sharedContext, QOffscreenSurface* surface) {
    m_ctx = ctx;
    m_sharedContext = sharedContext;
    m_surface = surface;
}

void MpvRenderThread::requestRender() {
    QMutexLocker locker(&m_mutex);
    m_shouldRender = true;
    m_waitCondition.wakeOne();
}

void MpvRenderThread::updateSize(const QSize& size) {
    if (size.width() < 10 || size.height() < 10) return;
    QMutexLocker locker(&m_mutex);
    m_videoSize = size;
    m_shouldRender = true;
    m_waitCondition.wakeOne();
}

void MpvRenderThread::stop() {
    {
        QMutexLocker locker(&m_mutex);
        if (!m_active) return;
        m_active = false;
        m_waitCondition.wakeOne();
    }
    if (isRunning()) wait();
}

void MpvRenderThread::cleanup() {
    delete m_renderFbo;
    m_renderFbo = nullptr;
    delete m_displayFbo;
    m_displayFbo = nullptr;
    delete m_glContext;
    m_glContext = nullptr;
}

void MpvRenderThread::run() {
    if (!m_surface || !m_sharedContext) {
        qWarning() << "Not initialized";
        return;
    }

    m_glContext = new QOpenGLContext();
    m_glContext->setFormat(m_surface->format());
    m_glContext->setShareContext(m_sharedContext);

    if (!m_glContext->create()) {
        qWarning() << "Failed to create GL context";
        return;
    }

    if (!m_glContext->makeCurrent(m_surface)) {
        qWarning() << "Failed to make current";
        return;
    }

    renderLoop();
    m_glContext->doneCurrent();
}

void MpvRenderThread::renderLoop() {
    while (true) {
        QSize size;
        {
            QMutexLocker locker(&m_mutex);
            while (!m_shouldRender) {
                if (!m_active) return;
                m_waitCondition.wait(&m_mutex);
            }
            if (!m_active) return;
            m_shouldRender = false;
            size = m_videoSize;
        }

        if (!m_ctx || size.isEmpty()) continue;

        bool needRecreate = !m_renderFbo || m_renderFbo->size() != size;
        if (needRecreate) {
            delete m_renderFbo;
            delete m_displayFbo;
            m_renderFbo = new QOpenGLFramebufferObject(size);
            m_displayFbo = new QOpenGLFramebufferObject(size);
        }

        mpv_opengl_fbo mpfbo{
            .fbo = static_cast<int>(m_renderFbo->handle()),
            .w = size.width(),
            .h = size.height(),
            .internal_format = 0
        };
        int flip_y = 0;
        mpv_render_param params[] = {
            {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
            {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
            {MPV_RENDER_PARAM_INVALID, nullptr}
        };
        mpv_render_context_render(m_ctx, params);

        QOpenGLFunctions* f = m_glContext->functions();
        f->glFinish();

        {
            QMutexLocker locker(&m_mutex);
            std::swap(m_renderFbo, m_displayFbo);
        }

        emit frameRendered();
    }
}
