#ifndef CALC_INTERFACE_SETTINGS_H
#define CALC_INTERFACE_SETTINGS_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class InterfaceSettings : public QWidget {
Q_OBJECT

public:
    InterfaceSettings(QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *label = new QLabel(" ", this);
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(label);

        setLayout(layout);
    }
};

#endif
