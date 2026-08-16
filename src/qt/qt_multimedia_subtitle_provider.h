#include "i_subtitle_provider.h"

class QtMultimediaSubtitleProvider : public ISubtitleProvider {

    public:
        QList<ISubtitleProvider::Track> getSubtitleTracks(const QString& videoPath) override;
        QString getSubtitleContent(const QString& videoPath, const Track& track) override;

};
