#pragma once

#include <QObject>

class IVideoPlayer{

    public:
        virtual ~IVideoPlayer() = default;

        virtual void loadVideo(const QString& path) = 0;
        virtual void togglePlayPause() = 0;
        virtual void setVolume(double volume) = 0;
        virtual void seek(double position) = 0;

        virtual double getPosition() const = 0;
        virtual double getDuration() const = 0;
};
