#ifndef CALC_INTERFACE_POKER_DISPLAY_H
#define CALC_INTERFACE_POKER_DISPLAY_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPixmap>

#include "poker.h"

namespace calc {

    class PokerDisplayWidget : public QWidget {
    Q_OBJECT

        QLabel *imageLabel;
        QLineEdit *lineEdit;

    public:
        PokerDisplayWidget(QWidget *parent = nullptr) : QWidget(parent) {

            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->setAlignment(Qt::AlignCenter);

            imageLabel = new QLabel(this);
            imageLabel->setFixedSize(120, 120);
            imageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setPixmap(QPixmap("img/poker_empty.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

            lineEdit = new QLineEdit(this);
            lineEdit->setFixedSize(42, 28);
            lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lineEdit->setStyleSheet(
                    "QLineEdit {"
                    "    border: 1px solid #BBBBBB;"
                    "    border-radius: 8px;"
                    "    background-color: #FFFFFF;"
                    "    padding: 0 8px;"
                    "    font-size: 12px;"
                    "    color: black;"
                    "}"
                    "QLineEdit:hover {"
                    "    border: 1px solid #FEFDFC;"
                    "    background-color: #FEFDFC;"
                    "}"
                    "QLineEdit:focus {"
                    "    border: 1px solid #BBBBBB;"
                    "    background-color: #FEFDFC;"
                    "}"
            );
            lineEdit->setAlignment(Qt::AlignCenter);

            QHBoxLayout *hLayout = new QHBoxLayout();
            hLayout->addWidget(lineEdit);
            hLayout->setAlignment(Qt::AlignCenter);

            layout->addWidget(imageLabel);
            layout->addItem(new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Minimum));
            layout->addLayout(hLayout);

            setLayout(layout);

            connect(lineEdit, &QLineEdit::textChanged, this, &PokerDisplayWidget::onTextChanged);
        }

        void lockEdit() { lineEdit->setReadOnly(true); }

        void unlockEdit() { lineEdit->setReadOnly(false); }

        void setValue(int value) { lineEdit->setText(int_to_poker(value).c_str()); }

        int getValue() { return poker_to_int(lineEdit->text().toStdString()); }

    private slots:
        void onTextChanged(const QString &text) {
            int index = poker_to_int(text.toStdString());
            QString imagePath;
            if(index == -1)
                imagePath = "img/poker_empty.png";
            else
                imagePath = QString("img/poker_%1.png").arg(index);
            imageLabel->setPixmap(QPixmap(imagePath).scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    };

}

#endif
