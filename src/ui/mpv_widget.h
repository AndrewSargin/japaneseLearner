#pragma once
#include <QWidget>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <qwidget.h>

class Mpv : public QWidget {

    public:
        Mpv(QWidget *parent = nullptr);
        ~Mpv();
        void loadVideo(const std::string& path);
        double getCurrentTime();
    private:
        mpv_handle* mpv;    

};
