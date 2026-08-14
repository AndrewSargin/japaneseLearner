#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSize>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <mpv/render_gl.h>

class MpvRenderThread : public QThread {
    Q_OBJECT
public:
    explicit MpvRenderThread(QObject* parent = nullptr);
    ~MpvRenderThread();

    void prepare(mpv_render_context* ctx, QOpenGLContext* sharedContext, QOffscreenSurface* surface);
    void requestRender();
    void updateSize(const QSize& size);
    void stop();
    void cleanup();

    QMutex* mutex() { return &m_mutex; }
    QOpenGLFramebufferObject* displayFbo() { return m_displayFbo; }

signals:
    void frameRendered();

protected:
    void run() override;

private:
    void renderLoop();

    mpv_render_context* m_ctx = nullptr;
    QOpenGLContext* m_sharedContext = nullptr;
    QOffscreenSurface* m_surface = nullptr;
    QOpenGLContext* m_glContext = nullptr;

    QOpenGLFramebufferObject* m_renderFbo = nullptr;
    QOpenGLFramebufferObject* m_displayFbo = nullptr;

    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QSize m_videoSize;
    bool m_shouldRender = false;
    bool m_active = true;
};
