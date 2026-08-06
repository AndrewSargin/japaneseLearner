// Troublemaking sentence example: -（スバル）お～ 言ってくれたな
// For ipadic to work

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <qqml.h>
#include "qt/mpv_widget.h"
#include "qt/engine_qt.h"
#include "qt/player_qt.h"


int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    std::string unidic_path = "-d /usr/lib/mecab/dic/unidic";


    EngineQt* engineQt = new EngineQt(db_path, unidic_path, "unidic");

    PlayerQt* playerQt = new PlayerQt();
    
    qmlRegisterType<MpvWidget>("JapaneseLearner", 1, 0, "MpvWidget");

    QQmlApplicationEngine qmlEngine;
    qmlEngine.rootContext()->setContextProperty("engine", engineQt);
    qmlEngine.rootContext()->setContextProperty("player", playerQt);

    qmlEngine.load(QUrl("qrc:/JapaneseLearner/src/ui/main.qml"));
    return app.exec();
    
}

/*int main () {

    // Paths used for testing
    std::string testPath = "/home/andrew/projects/japanese-learner/data/test.srt"; 
    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    std::string unidic_path = "-d /usr/lib/mecab/dic/unidic";

    // Construct engine
    Engine engine(db_path, unidic_path, "unidic");
    engine.loadSubtitles(testPath);
    for (double i = 0; i < 5425.957; i += 0.1){
    std::string phrase = engine.getPhraseAtTime(i);
    engine.printCurrentWords(phrase);
    }
    
    return 0;
}
*/
