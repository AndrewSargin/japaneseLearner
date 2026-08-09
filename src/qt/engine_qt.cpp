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

void EngineQt::loadOriginalSubtitles(const QString& path){
    QUrl url(path);
    engine.loadOriginalSubtitles(url.toLocalFile().toStdString());
}

void EngineQt::loadTranslationSubtitles(const QString& path){
    QUrl url(path);
    engine.loadTranslationSubtitles(url.toLocalFile().toStdString());
}

QStringList EngineQt::getPhraseAtTime(double time){
    std::vector<std::string> phrases = engine.getPhraseAtTime(time);
    QStringList result;
    for (std::string phrase : phrases) {
        result.append(QString::fromStdString(phrase));
    }
    return result;
}

QVariantList EngineQt::getWordsAtTime(double time) {
    std::string phrase = engine.getPhraseAtTime(time)[0];
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
