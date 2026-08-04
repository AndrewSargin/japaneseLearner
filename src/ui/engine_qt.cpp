#include "engine_qt.h"
#include <iostream>

EngineQt::EngineQt(const std::string& db_path,
                   const std::string& dict_path,
                   const std::string& dictionary,
                   QObject* parent) :
                   engine(db_path, dict_path, dictionary),
                   QObject(parent){
    //For now
    loadSubtitles("sdfs");
}

void EngineQt::loadSubtitles(const QString& path){
    // For now
    std::string pathstr = "/home/andrew/projects/japanese-learner/data/test.srt";
    engine.loadSubtitles(pathstr);
    //engine.loadSubtitles(path.toStdString());
}

QString EngineQt::getPhraseAtTime(double time){
    std::string phrase = engine.getPhraseAtTime(time);
    return QString::fromStdString(phrase);
}
