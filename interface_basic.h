#ifndef CALC_INTERFACE_BASIC_H
#define CALC_INTERFACE_BASIC_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <cstdlib>
#include "interface_poker_display.h"
#include "solver.h"
#include "style.h"

namespace calc {
    class InterfaceBasic : public QWidget {
    Q_OBJECT

        PokerDisplayWidget *pokers[4];
        QLineEdit *resultLineEdit;

    public:
        InterfaceBasic(QWidget *parent = nullptr) : QWidget(parent) {

            QVBoxLayout *mainLayout = new QVBoxLayout(this);

            QHBoxLayout *pokerLayout = new QHBoxLayout();
            pokerLayout->setAlignment(Qt::AlignCenter);

            for (auto &poker: pokers) {
                poker = new PokerDisplayWidget(this);
                pokerLayout->addWidget(poker);
            }

            QHBoxLayout *buttonLayout = new QHBoxLayout();

            QPushButton *randomButton = new QPushButton("随机取数", this);
            randomButton->setStyleSheet(Style::sheet("button_white"));
            connect(randomButton, &QPushButton::clicked, this, &InterfaceBasic::randomNumbers);
            buttonLayout->addWidget(randomButton);

            QPushButton *solveButton = new QPushButton("求解", this);
            solveButton->setStyleSheet(Style::sheet("button_blue"));
            connect(solveButton, &QPushButton::clicked, this, &InterfaceBasic::solveProblem);
            buttonLayout->addWidget(solveButton);

            resultLineEdit = new QLineEdit(this);
            resultLineEdit->setStyleSheet(Style::sheet("edit_box"));
            resultLineEdit->setReadOnly(true);
            buttonLayout->addWidget(resultLineEdit);

            mainLayout->addLayout(pokerLayout);
            mainLayout->addLayout(buttonLayout);

            setLayout(mainLayout);
        }

    private slots:

        void randomNumbers() {
            srand(time(nullptr));
            for (auto &poker: pokers) {
                poker->setValue(rand() % 13 + 1);
            }
        }

        void solveProblem() {
            solver solver_instance(4, 24);
            std::vector<rational> input_operand(4);
            for (int i = 0; i < 4; ++i) {
                if(pokers[i]->getValue() >= 1 && pokers[i]->getValue() <= 13)
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
            if(result)
                resultLineEdit->setText(answer.c_str());
            else
                resultLineEdit->setText("无解");
        }
    };


}

#endif
