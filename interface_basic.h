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

namespace calc {
    class PageBasic : public QWidget {
    Q_OBJECT

        PokerDisplayWidget *pokers[4];
        QLineEdit *resultLineEdit;

    public:
        PageBasic(QWidget *parent = nullptr) : QWidget(parent) {

            QVBoxLayout *mainLayout = new QVBoxLayout(this);

            QHBoxLayout *pokerLayout = new QHBoxLayout();
            pokerLayout->setAlignment(Qt::AlignCenter);

            for (auto &poker: pokers) {
                poker = new PokerDisplayWidget(this);
                pokerLayout->addWidget(poker);
            }

            QHBoxLayout *buttonLayout = new QHBoxLayout();

            QPushButton *randomButton = new QPushButton("随机取数", this);
            randomButton->setStyleSheet(
                    "QPushButton {"
                    "    height: 32px;"
                    "    width: 120px;"
                    "    border: 1px solid #BBBBBB;"
                    "    background-color: #FFFFFF;"
                    "    border-radius: 8px;"
                    "    color: #000000;"
                    "    font-size: 14px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #FEFDFC;"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: #FEFDFC;"
                    "    color: #505050;"
                    "}"
            );
            connect(randomButton, &QPushButton::clicked, this, &PageBasic::randomNumbers);
            buttonLayout->addWidget(randomButton);

            QPushButton *solveButton = new QPushButton("求解", this);
            solveButton->setStyleSheet(
                    "QPushButton {"
                    "    height: 32px;"
                    "    width: 120px;"
                    "    border: none;"
                    "    background-color: #0067C0;"
                    "    border-radius: 8px;"
                    "    color: #FFFFFF;"
                    "    font-size: 14px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #1975C4;"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: #1975C4;"
                    "    color: #DDDDDD;"
                    "}"
            );
            connect(solveButton, &QPushButton::clicked, this, &PageBasic::solveProblem);
            buttonLayout->addWidget(solveButton);

            resultLineEdit = new QLineEdit(this);
            resultLineEdit->setStyleSheet(
                    "QLineEdit {"
                    "    height: 28px;"
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
