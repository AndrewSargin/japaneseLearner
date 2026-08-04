#include "main_window.h"
#include <iostream>
#include <qmainwindow.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    std::string testPath = "/home/andrew/projects/japanese-learner/data/test.srt"; 
    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    std::string unidic_path = "-d /usr/lib/mecab/dic/unidic";


    menuBar = new QMenuBar(this);
//    menuBar->hide();
    menuBar->setGeometry(0, 0, width(), 25);
    menuBar->raise();

    hideMenuTimer = new QTimer(this);
    hideMenuTimer->setSingleShot(true);
    connect(hideMenuTimer, &QTimer::timeout, menuBar, &QMenuBar::hide);

    setMouseTracking(true);
    //m_mpv = new MpvWidget(this);
    //setCentralWidget(m_mpv);
    //m_mpv->setMouseTracking(true);
    
    engine = new Engine(db_path, unidic_path, "unidic");
    engine->loadSubtitles(testPath);

    //connect(m_mpv, SIGNAL(updateTime(double)), this, SLOT(updateSubtitle(double)));

}

MainWindow::~MainWindow(){

};

void MainWindow::updateSubtitle(double time){
    //double time = mpv->getCurrentTime();
    std::cout << "update" << std::endl;
    auto phrase = engine->getPhraseAtTime(time);
    //m_mpv->setSubtitle(phrase);
    engine->printCurrentWords(phrase);

}

void MainWindow::mouseMoveEvent(QMouseEvent* event){

    if(event->pos().y() < 50) {
        menuBar->show();
        hideMenuTimer->stop();
    }
    else {
        hideMenuTimer->start(2000);
    }
}
