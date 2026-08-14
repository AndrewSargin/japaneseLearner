#pragma once
#include <QObject>

class VideoPlayer;
class MpvWidget;

class PlayerQt : public QObject {

    Q_OBJECT

    Q_PROPERTY(double position READ getPosition NOTIFY positionChanged)
    Q_PROPERTY(double duration READ getDuration NOTIFY durationChanged)

    public: PlayerQt(QObject* parent = nullptr);
        Q_INVOKABLE void setWidget(VideoPlayer* player);
        Q_INVOKABLE void loadVideo(const QString& videoPath);
        Q_INVOKABLE void togglePlayPause();
        Q_INVOKABLE void setVolume(double volume);
        Q_INVOKABLE void seek(double position);
        //Q_INVOKABLE void setSpeed(double speed);
        

        double getPosition() const;
        double getDuration() const;
    
    Q_SIGNALS:
        void positionChanged();
        void durationChanged();

    private:

        VideoPlayer* m_player = nullptr;
        //MpvWidget* mpv = nullptr;
        double m_position = 0.0;
        double m_duration = 0.0;

    private Q_SLOTS:
        void onDurationChanged(double value);
        void onPositionChanged(double value); 

};
