#include "engine_qt.h"
#include <QUrl>

EngineQt::EngineQt(const std::string& db_path,
                   const std::string& dict_path,
                   const std::string& dictionary,
                   QObject* parent) :
                   engine(db_path, dict_path, dictionary),
                   QObject(parent){
}

void EngineQt::loadSubtitles(const QString& path){
    QUrl url(path);
    engine.loadSubtitles(url.toLocalFile().toStdString());
}

QString EngineQt::getPhraseAtTime(double time){
    std::string phrase = engine.getPhraseAtTime(time);
    return QString::fromStdString(phrase);
}
