#include <QString>

class ISubtitleProvider{

    public: 
        struct Track {
            int index;
            QString language;
            QString format;
            QString title;
        };

        virtual ~ISubtitleProvider() = default;
        virtual QList<Track> getSubtitleTracks(const QString& videoPath) = 0;
        virtual QString getSubtitleContent(const QString& videoPath, const Track& track) = 0;
};
