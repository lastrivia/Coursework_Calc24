#ifndef CALC_INTERFACE_ONLINE_GAME_H
#define CALC_INTERFACE_ONLINE_GAME_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>


// this module still has bugs, and is removed in current release version


// a placeholder:
class PageOnlineGame : public QWidget {
Q_OBJECT

public:
    PageOnlineGame(QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *label = new QLabel("This module still has bugs, and is removed in current release version.", this);
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(label);

        setLayout(layout);
    }
};

#endif
