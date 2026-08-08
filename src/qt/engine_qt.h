#pragma once
#include <QObject>
#include <qcontainerfwd.h>
#include <qtmetamacros.h>
#include "../engine/engine.h"

class EngineQt : public QObject {
    Q_OBJECT

    public:
        explicit EngineQt(const std::string& db_path,
                          const std::string& dict_path,
                          const std::string& dictionary,
                          QObject* parent = nullptr);
        Q_INVOKABLE QVariantList getWordsAtTime(double time);

    public slots:
        void loadSubtitles(const QString& path);
        QString getPhraseAtTime(double time);

    private:
        Engine engine;
};
