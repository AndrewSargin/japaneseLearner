#include "mpv_widget.h"
#include <clocale>
#include <cstring>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QLabel>
#include <mpv/client.h>
#include <qnamespace.h>
#include <qtmetamacros.h>

static void wakeup(void *ctx){
    QMetaObject::invokeMethod((MpvWidget*)ctx, "on_mpv_events", Qt::QueuedConnection);
}

static void *get_proc_address(void *ctx, const char *name){
    Q_UNUSED(ctx);
    QOpenGLContext *glctx = QOpenGLContext::currentContext();
    if (!glctx)
        return nullptr;
    return reinterpret_cast<void *>(glctx->getProcAddress(QByteArray(name)));

}

MpvWidget::MpvWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    setlocale(LC_NUMERIC, "C");

    mpv = mpv_create();
    if (!mpv)
        std::cout << "could not create mpv context" << std::endl;

    mpv_set_option_string(mpv, "terminal", "yes");
    mpv_set_option_string(mpv, "msg-level", "all=v");
    mpv_set_option_string(mpv, "vo", "libmpv");

    if (mpv_initialize(mpv) < 0)
        std::cout << "could not initialize mpv context" << std::endl;

    //mpv_set_option_string(mpv, "hwdec", "auto");
    


    mpv_set_wakeup_callback(mpv, wakeup, this);

}

MpvWidget::~MpvWidget(){

    makeCurrent();
    if (mpv_gl)
        mpv_render_context_free(mpv_gl);
    mpv_terminate_destroy(mpv);
}

void MpvWidget::loadVideo(const std::string& path){

    const char *cmd[] = {"loadfile", path.c_str(), nullptr};
    mpv_command(mpv, cmd);

}

double MpvWidget::getCurrentTime(){
    double time = 0;
    mpv_get_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &time);

    return time;
}

void MpvWidget::maybeUpdate(){

    if (window()->isMinimized()){
        makeCurrent();
        paintGL();
        context()->swapBuffers(context()->surface());
        doneCurrent();
    }
    else {
        update();
    }
}

void MpvWidget::initializeGL(){

    mpv_opengl_init_params gl_init_params[1] = {get_proc_address, nullptr};
    mpv_render_param params[]{
        {MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
        {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    if (mpv_render_context_create(&mpv_gl, mpv, params))
        std::cout << "failed to create mpv GL context" << std::endl;
    mpv_render_context_set_update_callback(mpv_gl, MpvWidget::onUpdate, reinterpret_cast<void *>(this));
    
    loadVideo("/home/andrew/Downloads/Rezero.mkv");

}

void MpvWidget::onUpdate(void *ctx){
    QMetaObject::invokeMethod((MpvWidget*)ctx, "maybeUpdate");
}

void MpvWidget::paintGL(){
    mpv_opengl_fbo mpfbo{static_cast<int>(defaultFramebufferObject()), width(), height(), 0};
    int flip_y{1};

    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
        {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };
        
    mpv_render_context_render(mpv_gl, params);

    updateTime(getCurrentTime());

    if(!subtitleText.empty()){
        std::cout << "in if" << std::endl;
        QPainter painter(this);
        painter.setFont(QFont("Arial", 24));
        painter.setPen(Qt::white);
        QRect rect(0, height() - 80, width(), 80);
        painter.drawText(rect, Qt::AlignCenter, QString::fromStdString(subtitleText));
    }
}

void MpvWidget::on_mpv_events(){

    while (mpv) {
        mpv_event *event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }
        handle_mpv_event(event);
    }

}

void MpvWidget::handle_mpv_event(mpv_event *event){

    switch (event->event_id) {
        case MPV_EVENT_PROPERTY_CHANGE: {
            mpv_event_property  *prop = (mpv_event_property *)event->data;
            if (strcmp(prop->name, "time-pos") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double time = *(double *)prop->data;
                    Q_EMIT positionChanged(time);
                }
            }
            else if (strcmp(prop->name, "duration") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double time = *(double *)prop->data;
                    Q_EMIT durationChanged(time);
              }
            }
            break;
        }
        default: ;
    }

            
}

void MpvWidget::setSubtitle(const std::string& text){
    subtitleText = text;
    std::cout << "setSubs" << std::endl;
    update();

}


