#pragma once
#include "mpv_widget.h"
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <mpv/render_gl.h>

class MpvWidget;

class MpvRenderer : public QQuickFramebufferObject::Renderer {

    public:
        MpvRenderer(MpvWidget* widget);
        ~MpvRenderer();

        void render() override;
        QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

    private:
        MpvWidget* widget;
        static void* get_proc_address(void* ctx, const char* name);
};
