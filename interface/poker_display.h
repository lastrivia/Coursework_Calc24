#ifndef CALC_POKER_DISPLAY_H
#define CALC_POKER_DISPLAY_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPixmap>
#include <random>

#include "../algorithm/poker.h"
#include "style.h"

namespace calc {

    class PokerDisplayWidget : public QWidget {
    Q_OBJECT

        QLabel *imageLabel;
        QLineEdit *lineEdit;
        QString suitName;
        std::mt19937 *randomEngine;

    public:
        PokerDisplayWidget(QWidget *parent = nullptr) : QWidget(parent) {

            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->setAlignment(Qt::AlignCenter);

            imageLabel = new QLabel(this);
            imageLabel->setFixedSize(120, 120);
            imageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setPixmap(
                    QPixmap("img/poker_unknown.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

            lineEdit = new QLineEdit(this);
            lineEdit->setStyleSheet(Style::sheet("edit_box"));
            lineEdit->setFixedSize(42, 28);
            lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lineEdit->setAlignment(Qt::AlignCenter);

            QHBoxLayout *hLayout = new QHBoxLayout();
            hLayout->addWidget(lineEdit);
            hLayout->setAlignment(Qt::AlignCenter);

            layout->addWidget(imageLabel);
            layout->addItem(new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Minimum));
            layout->addLayout(hLayout);

            std::random_device rd;
            randomEngine = new std::mt19937(rd());

            setLayout(layout);

            connect(lineEdit, &QLineEdit::textChanged, this, &PokerDisplayWidget::onTextChanged);
        }

        ~PokerDisplayWidget() override {
            delete randomEngine;
        }

        void lockEdit() { lineEdit->setReadOnly(true); }

        void unlockEdit() { lineEdit->setReadOnly(false); }

        void setValue(int value) { lineEdit->setText(int_to_poker(value).c_str()); }

        int getValue() { return poker_to_int(lineEdit->text().toStdString()); }

    private slots:

        void onTextChanged(const QString &text) {
            int index = poker_to_int(text.toStdString());
            QString imagePath;

            if (index == -1)
                imagePath = "img/poker_unknown.png";
            else {
                std::uniform_int_distribution<> dis(0, 3);
                int suit = dis(*randomEngine);
                switch (suit) {
                    case 0:
                        suitName = "clubs";
                        break;
                    case 1:
                        suitName = "diamonds";
                        break;
                    case 2:
                        suitName = "spades";
                        break;
                    case 3:
                        suitName = "hearts";
                        break;
                    default:
                        break;
                }
                imagePath = QString("img/poker_%1_%2.png").arg(suitName).arg(index);
            }
            imageLabel->setPixmap(QPixmap(imagePath).scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    };

}

#endif
