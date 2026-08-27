#pragma once
#include <QObject>
#include <qcontainerfwd.h>
#include <qtmetamacros.h>
#include "../engine/engine.h"

class EngineQt : public QObject {
    Q_OBJECT
        Q_PROPERTY(QVariantList subtitleTracks READ getSubtitleTracks NOTIFY subtitleTracksChanged)

    public:
        explicit EngineQt(const std::string& db_path,
                          const std::string& dict_path,
                          const std::string& dictionary,
                          QObject* parent = nullptr);

        Q_INVOKABLE QVariantList getWordsAtTime(double time);
        Q_INVOKABLE QVariantList getSubtitleTracks() const;
        Q_INVOKABLE void prepareVideo(const QString& path);
        Q_INVOKABLE void loadSubtitles(int idx1, int idx2);

    public slots:
        QStringList getPhraseAtTime(double time);
        void addExternalSubtitleToList(const QString& path);

    Q_SIGNALS:
        void subtitleTracksChanged();

    private:
        Engine engine;
};
