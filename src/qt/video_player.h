#pragma once

#include <QMediaPlayer>
#include <QQuickItem>
#include "i_video_player.h"

class VideoPlayer : public QMediaPlayer, public IVideoPlayer {
    Q_OBJECT
    QML_ELEMENT

    public:
        explicit VideoPlayer(QObject* parent = nullptr);
        ~VideoPlayer();

        Q_INVOKABLE void loadVideo(const QString& path) override;
        Q_INVOKABLE void setVolume(double volume) override;
        Q_INVOKABLE void seek(double position) override;
        Q_INVOKABLE void togglePlayPause() override;

        Q_INVOKABLE double getPosition() const override;
        Q_INVOKABLE double getDuration() const override;

    Q_SIGNALS:
        void positionChanged();
        void durationChanged();

    private:
        QAudioOutput* m_audioOutput = nullptr;

    private Q_SLOTS:
        void onQMediaPositionChanged(qint64);
        void onQMediaDurationChanged(qint64);

};
