#include "mpv_widget.h"
#include "mpv_renderer.h"
#include <clocale>
#include <iostream>
#include <mpv/client.h>
#include <qnamespace.h>
#include <qquickframebufferobject.h>
#include <QTimer>

void MpvWidget::wakeup(void* ctx) {
    QMetaObject::invokeMethod((MpvWidget*)ctx, "onMpvEvents", Qt::QueuedConnection);

}

MpvWidget::MpvWidget(QQuickItem* parent) : QQuickFramebufferObject(parent)
{
    setlocale(LC_NUMERIC, "C");
    setMirrorVertically(true);
    mpv = mpv_create();
    if (!mpv)
        std::cout << "could not create mpv context" << std::endl;

    mpv_set_option_string(mpv, "vo", "libmpv");

    if (mpv_initialize(mpv) < 0)
        std::cout << "could not initialize mpv context" << std::endl;

    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);

}

MpvWidget::~MpvWidget(){

    if (mpv_gl)
        mpv_render_context_free(mpv_gl);
    mpv_terminate_destroy(mpv);
}

QQuickFramebufferObject::Renderer* MpvWidget::createRenderer() const {
    return new MpvRenderer(const_cast<MpvWidget*>(this));
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

void MpvWidget::onUpdate(void *ctx){
    QMetaObject::invokeMethod((MpvWidget*)ctx, "maybeUpdate");
}

void MpvWidget::onMpvEvents(){
    
    while (mpv) {
        mpv_event* event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) break;
        handleMpvEvent(event);
    }

}

void MpvWidget::handleMpvEvent(mpv_event *event){

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
    update();
}

void MpvWidget::maybeUpdate(){
    update();
}


