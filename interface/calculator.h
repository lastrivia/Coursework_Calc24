#ifndef CALC_CALCULATOR_H
#define CALC_CALCULATOR_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <cstdlib>
#include "../algorithm/solver.h"
#include "../algorithm/random.h"
#include "poker_display.h"
#include "style.h"
#include "timed_game.h"

namespace calc {
    class InterfaceCalculator : public QWidget {
    Q_OBJECT

        PokerDisplayWidget *pokers[4];
        QLineEdit *resultLineEdit;

        InterfaceTimedGame *timedGameWidgetLink;

    public:
        InterfaceCalculator(QWidget *parent = nullptr) : QWidget(parent) {

            QVBoxLayout *mainLayout = new QVBoxLayout(this);

            QHBoxLayout *pokerLayout = new QHBoxLayout();
            pokerLayout->setAlignment(Qt::AlignCenter);

            for (auto &poker: pokers) {
                poker = new PokerDisplayWidget(this);
                pokerLayout->addWidget(poker);
            }

            QHBoxLayout *buttonLayout = new QHBoxLayout();

            QPushButton *randomButton = new QPushButton("随机取数", this);
            randomButton->setStyleSheet(Style::QSSKey("button_white"));
            randomButton->setIcon(QIcon("img/icon_button_refresh.png"));
            randomButton->setIconSize(QSize(20, 20));
            randomButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(randomButton, &QPushButton::clicked, this, &InterfaceCalculator::randomNumbers);
            buttonLayout->addWidget(randomButton);

            QPushButton *solveButton = new QPushButton("求解", this);
            solveButton->setStyleSheet(Style::QSSKey("button_blue"));
            solveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(solveButton, &QPushButton::clicked, this, &InterfaceCalculator::solveProblem);
            buttonLayout->addWidget(solveButton);

            resultLineEdit = new QLineEdit(this);
            resultLineEdit->setStyleSheet(Style::QSSKey("edit_box"));
            resultLineEdit->setReadOnly(true);
            buttonLayout->addWidget(resultLineEdit);

            mainLayout->addLayout(pokerLayout);
            mainLayout->addLayout(buttonLayout);

            setLayout(mainLayout);

            timedGameWidgetLink = nullptr;
        }

    private slots:

        void randomNumbers() {
            std::vector<rational> v = randomized_integers(4, 1, 13);
            for (int i = 0; i < 4; ++i)
                pokers[i]->setValue((int) v[i]);
        }

        void solveProblem() {
            solver solver_instance(4, 24);
            std::vector<rational> input_operand(4);
            for (int i = 0; i < 4; ++i) {
                if (pokers[i]->getValue() >= 1 && pokers[i]->getValue() <= 13)
                    input_operand[i] = pokers[i]->getValue();
                else {
                    resultLineEdit->setText("请输入 4 个 1~10 的数字或 J/Q/K");
                    return;
                }
            }

            std::string answer;
            bool result;
            solver_instance.set_operands(input_operand);
            result = solver_instance.solve(answer);
            if (result)
                resultLineEdit->setText(answer.c_str());
            else
                resultLineEdit->setText("无解");


            if(timedGameWidgetLink && timedGameWidgetLink->gameStarted()) {
                auto game_operands = timedGameWidgetLink->getOperands();
                if(same_elements(input_operand, game_operands))
                    timedGameWidgetLink->setCheatFlag();
            }
        }

    public:

        void connectTimedGame(InterfaceTimedGame *timedGame) {
            timedGameWidgetLink = timedGame;
        }
    };


}

#endif
