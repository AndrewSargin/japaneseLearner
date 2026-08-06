#include "player_qt.h"
#include <qobject.h>
#include <iostream>

PlayerQt::PlayerQt(QObject* parent) :
                   QObject(parent){

}

void PlayerQt::loadVideo(const QString& videoPath){

    std::string path = videoPath.toStdString();
    std::cout << "load";
    if (mpv != nullptr)
        mpv->loadVideo(path);
    else throw ("cannot load video: mpv is nullptr");

}

void PlayerQt::setWidget(MpvWidget* mpv){
    this->mpv = mpv;
}
