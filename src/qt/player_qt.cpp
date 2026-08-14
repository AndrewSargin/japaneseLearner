#include "player_qt.h"
#include "mpv_widget.h"
#include "video_player.h"
#include <qobject.h>
#include <iostream>

PlayerQt::PlayerQt(QObject* parent) :
                   QObject(parent){

}

void PlayerQt::loadVideo(const QString& videoPath){

    std::string path = videoPath.toStdString();
    std::cout << path << std::endl;
    
    std::cout << "load";
    //if (mpv != nullptr)
    //   mpv->loadVideo(path);
    //else throw ("cannot load video: mpv is nullptr");

}

void PlayerQt::setWidget(VideoPlayer* player){

    m_player = player;

    /*connect(mpv, &MpvWidget::positionChanged, this, &PlayerQt::onPositionChanged);
    connect(mpv, &MpvWidget::durationChanged, this, &PlayerQt::onDurationChanged);*/
}

void PlayerQt::togglePlayPause(){
   m_player->togglePlayPause();
}

void PlayerQt::setVolume(double volume){
    m_player->setVolume(volume);

}

void PlayerQt::seek(double position){
    m_player->setVideoPosition(position); 
}

double PlayerQt::getDuration() const{
    return m_duration;
}

double PlayerQt::getPosition() const{
    return m_position;
}

void PlayerQt::onDurationChanged(double value){
    m_duration = value;
    Q_EMIT durationChanged();
}

void PlayerQt::onPositionChanged(double value){
    m_position = value;
    Q_EMIT positionChanged();
}
