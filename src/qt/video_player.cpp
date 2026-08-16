#include "video_player.h"

#include <QAudioOutput>
#include <QUrl>

VideoPlayer::VideoPlayer(QObject* parent) : QMediaPlayer(parent){

    m_audioOutput = new QAudioOutput(this);
    setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.25);

    connect(this, &QMediaPlayer::positionChanged, this, &VideoPlayer::onQMediaPositionChanged);
    connect(this, &QMediaPlayer::durationChanged, this, &VideoPlayer::onQMediaDurationChanged);

}

VideoPlayer::~VideoPlayer(){

}

void VideoPlayer::loadVideo(const QString& path){
    setSource(QUrl::fromLocalFile(path));
    play();
}

void VideoPlayer::setVolume(double volume){
    m_audioOutput->setVolume(volume / 100.0);
}

void VideoPlayer::seek(double position){
    QMediaPlayer::setPosition(static_cast<qint64>(position * 1000.0));
}

void VideoPlayer::togglePlayPause(){

    if(isPlaying())
        pause();
    else play();
}

double VideoPlayer::getPosition() const{
    return QMediaPlayer::position() / 1000.0;
}
double VideoPlayer::getDuration() const{
    return QMediaPlayer::duration() / 1000.0;
}

void VideoPlayer::onQMediaPositionChanged(qint64){
    Q_EMIT positionChanged();
}
void VideoPlayer::onQMediaDurationChanged(qint64){
    Q_EMIT durationChanged();
}
