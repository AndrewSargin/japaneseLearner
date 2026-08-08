#include "engine_qt.h"
#include <QUrl>
#include <qcontainerfwd.h>
#include <QVariantMap>

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

QVariantList EngineQt::getWordsAtTime(double time) {
    std::string phrase = engine.getPhraseAtTime(time);
    std::vector<Word> words = engine.getWordsFromPhrase(phrase);
    
    QVariantList result;

    for (auto& word : words) {
        if (word.kanji.length() != 0){
            QVariantMap map;
            map["kanji"] = QString::fromStdString(word.kanji);
            map["reading"] = QString::fromStdString(word.reading);
            map["meaning"] = QString::fromStdString(word.meanings.at(0).meaning);
            map["jlpt"] = word.jlptLevel;

            result.append(map);
        }
    }

    return result;
}
