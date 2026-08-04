#pragma once

#include <QMainWindow>
#include <qaction.h>
#include <QMenuBar>
#include <QTimer>
#include <QMouseEvent>
#include "mpv_widget.h"
#include "subtitle_widget.h"
#include "../engine/engine.h"

class MainWindow : public QMainWindow {
    
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private: 
        MpvWidget* m_mpv;
        QLabel* subWid;
        //SubtitleWidget* subWid;
        Engine* engine;
        QMenuBar* menuBar;
        QTimer* hideMenuTimer;
    private Q_SLOTS:
        void updateSubtitle(double time);
    protected:
        void mouseMoveEvent(QMouseEvent* event) override;
};
