#include "main_window.h"
#include <QLabel>
#include <QLayout>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    std::string testPath = "/home/andrew/projects/japanese-learner/data/test.srt"; 
    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    std::string unidic_path = "-d /usr/lib/mecab/dic/unidic";

    m_mpv = new MpvWidget(this);
    setCentralWidget(m_mpv);
    
    engine = new Engine(db_path, unidic_path, "unidic");
    engine->loadSubtitles(testPath);

    connect(m_mpv, SIGNAL(updateTime(double)), this, SLOT(updateSubtitle(double)));

}

MainWindow::~MainWindow(){

};

void MainWindow::updateSubtitle(double time){
    //double time = mpv->getCurrentTime();
    std::cout << "update" << std::endl;
    auto phrase = engine->getPhraseAtTime(time);
    m_mpv->setSubtitle(phrase);
    engine->printCurrentWords(phrase);

}
