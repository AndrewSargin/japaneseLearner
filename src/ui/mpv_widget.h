#pragma once
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <QQuickFramebufferObject>
#include <QQuickItem>

class MpvWidget : public QQuickFramebufferObject {

    friend class MpvRenderer;

    Q_OBJECT
    QML_ELEMENT
    public:
        MpvWidget(QQuickItem *parent = nullptr);
        ~MpvWidget();

        Renderer* createRenderer() const override;

        void loadVideo(const std::string& path);
        Q_INVOKABLE double getCurrentTime();
        void setSubtitle(const std::string& text);

    Q_SIGNALS:
        void durationChanged(int value);
        void positionChanged(int value);
        void updateTime(double time);

    private Q_SLOTS:
        void onMpvEvents();
        void maybeUpdate();

    private:
        mpv_handle* mpv;
        mpv_render_context* mpv_gl = nullptr;
        static void onUpdate(void *ctx);
        static void wakeup(void* ctx);
        std::string subtitleText;
        void handleMpvEvent(mpv_event *event);

};
