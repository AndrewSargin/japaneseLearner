#pragma once
#include <QWidget>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <QOpenGLWidget>
#include <qtmetamacros.h>

class MpvWidget : public QOpenGLWidget {

    Q_OBJECT
    public:
        MpvWidget(QWidget *parent = nullptr);
        ~MpvWidget();
        void loadVideo(const std::string& path);
        double getCurrentTime();
        void setSubtitle(const std::string& text);
    Q_SIGNALS:
        void durationChanged(int value);
        void positionChanged(int value);
        void updateTime(double time);
    private Q_SLOTS:
        void maybeUpdate();
        void on_mpv_events();
    private:
        mpv_handle* mpv;
        mpv_render_context* mpv_gl;
        static void onUpdate(void *ctx);
        std::string subtitleText;
        void handle_mpv_event(mpv_event *event);

    protected:
        void initializeGL() Q_DECL_OVERRIDE;
        void paintGL() Q_DECL_OVERRIDE;
};
