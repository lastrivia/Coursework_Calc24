#ifndef CALC_ONLINE_GAME_H
#define CALC_ONLINE_GAME_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

namespace calc {

    class InterfaceOnlineGame : public QWidget {
    Q_OBJECT

    public:
        InterfaceOnlineGame(QWidget *parent = nullptr) : QWidget(parent) {
            QLabel *label = new QLabel(
                    R"(当前模块仍在开发中... <br> <br> 您可以访问 <a href="https://github.com/lastrivia/Coursework_Calc24">Github</a> 以了解最新开发进度)",
                    this);
            label->setStyleSheet("font-size: 14px;");
            label->setOpenExternalLinks(true);
            label->setAlignment(Qt::AlignCenter);

            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->addWidget(label);

            setLayout(layout);
        }
    };

}

#endif
