#ifndef CALC_INTERFACE_H
#define CALC_INTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "interface_menu.h"
#include "interface_basic.h"
#include "interface_file.h"
#include "interface_timed_game.h"
#include "interface_online_game.h"
#include "interface_settings.h"

namespace calc {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {

            QWidget *centralWidget = new QWidget(this);
            setCentralWidget(centralWidget);
            QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

            MenuWidget *menuList = new MenuWidget(this);

            QStackedWidget *stackedWidget = new QStackedWidget(this);
            QWidget *pageBasic = new PageBasic;
            QWidget *pageFile = new PageFile;
            QWidget *pageTimedGame = new PageTimedGame;
            QWidget *pageOnlineGame = new PageOnlineGame;
            QWidget *pageSettings = new PageSettings;
            stackedWidget->addWidget(pageBasic);
            stackedWidget->addWidget(pageFile);
            stackedWidget->addWidget(pageTimedGame);
            stackedWidget->addWidget(pageOnlineGame);
            stackedWidget->addWidget(pageSettings);

            mainLayout->addWidget(menuList);
            mainLayout->addWidget(stackedWidget);
            mainLayout->setStretch(0, 1);
            mainLayout->setStretch(1, 3);

            connect(menuList, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);

            setWindowTitle("24-Point Calculator");
            resize(960, 600);
        }
    };

}

#endif
