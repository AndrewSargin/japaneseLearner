#pragma once
#include <QQuickItem>
#include <QSGImageNode>
#include <QSGTexture>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include "mpv_render_thread.h"
#include <QOffscreenSurface>

class MpvWidget : public QQuickItem {
    friend class MpvRenderThread;
    Q_OBJECT
    QML_ELEMENT
public:
    MpvWidget(QQuickItem* parent = nullptr);
    ~MpvWidget();

    Q_INVOKABLE double getCurrentTime();
    void loadVideo(const std::string& path);
    void togglePlayPause();
    void setVolume(double volume);
    void setVideoPosition(double position);

Q_SIGNALS:
    void positionChanged(double time);
    void durationChanged(double time);
    void needsUpdate();

private Q_SLOTS:
    void onMpvEvents();
    void onFrameReady();

private:
    static void wakeup(void* ctx);
    static void on_mpv_redraw(void* ctx);
    void handleMpvEvent(mpv_event* event);

    mpv_handle* mpv = nullptr;
    mpv_render_context* mpv_gl = nullptr;

    MpvRenderThread* m_renderThread = nullptr;
    QOffscreenSurface* m_surface = nullptr;
    bool m_threadStarted = false;
    bool m_glInitialized = false;

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
};
