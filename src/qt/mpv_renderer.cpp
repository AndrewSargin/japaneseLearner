#include "mpv_renderer.h"
#include <mpv/render.h>
#include <mpv/render_gl.h>
#include <qopenglcontext.h>
#include <qopenglframebufferobject.h>
#include <QOpenGLContext>
#include <iostream>
#include <QQuickWindow>

MpvRenderer::MpvRenderer(MpvWidget* w) : widget(w) {

}

MpvRenderer::~MpvRenderer(){
}

QOpenGLFramebufferObject* MpvRenderer::createFramebufferObject(const QSize& size) {

    if (!widget->mpv_gl) {
        mpv_opengl_init_params gl_init_params[1] = {get_proc_address, nullptr};
        mpv_render_param params []{
            {MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
            {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
            {MPV_RENDER_PARAM_INVALID, nullptr}
        };
        
        if (mpv_render_context_create(&widget->mpv_gl, widget->mpv, params) < 0)
            std::cout << "failed to initialize mpv GL context" << std::endl;
        mpv_render_context_set_update_callback(widget->mpv_gl, widget->onUpdate, widget);
    }

    return QQuickFramebufferObject::Renderer::createFramebufferObject(size);


}

void MpvRenderer::render() {
    widget->window()->beginExternalCommands();

    QOpenGLFramebufferObject* fbo = framebufferObject();
    mpv_opengl_fbo mpfbo{.fbo = static_cast<int>(fbo->handle()), .w = fbo->width(), .h = fbo->height(), .internal_format = 0};
    int flip_y{1};

    mpv_render_param params[] = {

    {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
    {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
    {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    mpv_render_context_render(widget->mpv_gl, params);

    widget->window()->endExternalCommands();

}

void* MpvRenderer::get_proc_address(void* ctx, const char* name){
    
    Q_UNUSED(ctx)

    QOpenGLContext* glctx = QOpenGLContext::currentContext();
    if (!glctx) return nullptr;

    return reinterpret_cast<void *>(glctx->getProcAddress(QByteArray(name)));
}
