#include "main_window.h"
#include <mpv/client.h>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    std::string testPath = "/home/andrew/projects/japanese-learner/data/test.srt"; 
    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    std::string unidic_path = "-d /usr/lib/mecab/dic/unidic";

    mpv = new Mpv(this);
    setCentralWidget(mpv);
    engine = new Engine(db_path, unidic_path, "unidic");
    engine->loadSubtitles(testPath);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSubtitle);
    timer->start(100);
}

MainWindow::~MainWindow(){

};

void MainWindow::updateSubtitle(){
    double time = mpv->getCurrentTime();
    auto phrase = engine->getPhraseAtTime(time);
    engine->printCurrentWords(phrase);

}
