#pragma once

#include <QMainWindow>
#include <qmainwindow.h>
#include "mpv_widget.h"
#include "../engine/engine.h"

class MainWindow : public QMainWindow {
    
    Q_OBJECT;

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private: 
        Mpv* mpv;
        Engine* engine;
        void updateSubtitle();
};
