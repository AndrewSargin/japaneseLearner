#pragma once
#include <QObject>
#include <qtmetamacros.h>
#include "mpv_widget.h"

class PlayerQt : public QObject {

    Q_OBJECT

    public: PlayerQt(QObject* parent = nullptr);

    public slots:
        void setWidget(MpvWidget* mpv);
        void loadVideo(const QString& videoPath);

    private:

        MpvWidget* mpv = nullptr;

};
