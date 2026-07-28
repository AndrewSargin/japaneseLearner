#include "mpv_widget.h"
#include <mpv/client.h>
#include <iostream>
#include <clocale>

Mpv::Mpv(QWidget *parent) : QWidget(parent)
{

    setlocale(LC_NUMERIC, "C");
    mpv = mpv_create();

    std::cout << "mpv ptr = " << mpv << std::endl;
    if(!mpv) {
        qFatal("mpv not created");
        return;
    }
        
    mpv_initialize(mpv);
    std::cout << "mpv init" << std::endl;
    
    loadVideo("/home/andrew/Downloads/Rezero.mkv");

}

Mpv::~Mpv(){

}

void Mpv::loadVideo(const std::string& path){

    int wid = winId();
    mpv_set_property(mpv, "wid", MPV_FORMAT_INT64, &wid);
    const char *cmd[] = {"loadfile", path.c_str(), nullptr};
    mpv_command(mpv, cmd);

}

double Mpv::getCurrentTime(){
    double time = 0;
    mpv_get_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &time);

    return time;
}
