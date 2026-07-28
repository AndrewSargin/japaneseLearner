// Troublemaking sentence example: -（スバル）お～ 言ってくれたな
// For ipadic to work

#include <QApplication>
#include "ui/main_window.h"

/*
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Japanese Learner");
    window.resize(1280, 720);
    
    return app.exec();
    
}*/

int main () {

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

