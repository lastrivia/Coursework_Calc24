#ifndef CALC_INTERFACE_H
#define CALC_INTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QTimer>
#include <QAbstractAnimation>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSurfaceFormat>

#include "menu.h"
#include "calculator.h"
#include "file_calculator.h"
#include "timed_game.h"
#include "online_game.h"
#include "settings.h"

#ifndef APP_VERSION     // defined in CMakeLists
#define APP_VERSION ""
#endif

namespace calc {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {

            QSurfaceFormat format;
            format.setSwapInterval(1);
            QSurfaceFormat::setDefaultFormat(format);

            setStyleSheet(Style::sheet("main_window"));

            QWidget *centralWidget = new QWidget(this);
            setCentralWidget(centralWidget);
            QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

            MenuWidget *menuList = new MenuWidget(this);

            QStackedWidget *stackedWidget = new QStackedWidget(this);
            QWidget *pageBasic = new InterfaceCalculator;
            QWidget *pageFile = new InterfaceFileCalculator;
            QWidget *pageTimedGame = new InterfaceTimedGame;
            QWidget *pageOnlineGame = new InterfaceOnlineGame;
            QWidget *pageSettings = new InterfaceSettings;

            QGraphicsOpacityEffect *effectBasic = new QGraphicsOpacityEffect(pageBasic);
            pageBasic->setGraphicsEffect(effectBasic);
            QGraphicsOpacityEffect *effectFile = new QGraphicsOpacityEffect(pageFile);
            pageFile->setGraphicsEffect(effectFile);
            QGraphicsOpacityEffect *effectTimedGame = new QGraphicsOpacityEffect(pageTimedGame);
            pageTimedGame->setGraphicsEffect(effectTimedGame);
            QGraphicsOpacityEffect *effectOnlineGame = new QGraphicsOpacityEffect(pageOnlineGame);
            pageOnlineGame->setGraphicsEffect(effectOnlineGame);
            QGraphicsOpacityEffect *effectSettings = new QGraphicsOpacityEffect(pageSettings);
            pageSettings->setGraphicsEffect(effectSettings);
            effectBasic->setOpacity(1.0);

            stackedWidget->addWidget(pageBasic);
            stackedWidget->addWidget(pageFile);
            stackedWidget->addWidget(pageTimedGame);
            stackedWidget->addWidget(pageOnlineGame);
            stackedWidget->addWidget(pageSettings);

            mainLayout->addWidget(menuList);
            mainLayout->addWidget(stackedWidget);
            mainLayout->setStretch(0, 1);
            mainLayout->setStretch(1, 3);

            connect(menuList, &QListWidget::currentRowChanged, this, [stackedWidget](int index) {

                stackedWidget->setCurrentIndex(index);
                QWidget *newWidget = stackedWidget->currentWidget();
                QGraphicsOpacityEffect *newEffect = qobject_cast<QGraphicsOpacityEffect*>(newWidget->graphicsEffect());

                QPropertyAnimation *animationFadeIn = new QPropertyAnimation(newEffect, "opacity");
                animationFadeIn->setDuration(200);
                animationFadeIn->setStartValue(0.5);
                animationFadeIn->setEndValue(1.0);
                animationFadeIn->start(QAbstractAnimation::DeleteWhenStopped);

                QPropertyAnimation *animationFloatUp = new QPropertyAnimation(newWidget, "geometry");
                QRect startRect = newWidget->geometry();
                startRect.moveTop(startRect.top() + 10);
                animationFloatUp->setDuration(300);
                animationFloatUp->setStartValue(startRect);
                animationFloatUp->setEndValue(newWidget->geometry());
                animationFloatUp->setEasingCurve(QEasingCurve::OutQuad);
                animationFloatUp->start(QAbstractAnimation::DeleteWhenStopped);
            });

            QString title = "24-Point Calculator", appVersion = APP_VERSION;
            if(!appVersion.isEmpty())
                title = title + " v" + appVersion;
            setWindowTitle(title);
            resize(960, 600);
        }
    };

}

#endif
