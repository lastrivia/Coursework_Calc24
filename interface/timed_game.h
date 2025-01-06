#ifndef CALC_TIMED_GAME_H
#define CALC_TIMED_GAME_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QStackedLayout>

#include "../algorithm/expression.h"
#include "../algorithm/solver.h"

namespace calc {

    class InterfaceTimedGame : public QWidget {
    Q_OBJECT

        int currentScore, highScore;
        int timeLeft;
        int correctCount, abandonCount;

        QTimer *timer;
        QLabel *timerIconLabel, *timerLabel, *scoreLabel, *highScoreLabel;
        QLineEdit *answerInput;
        QLabel *answerCheckIcon;
        QLabel *scoreResultLabel, *scoreChangeLabel;
        QPushButton *startButton, *submitButton, *abandonButton;
        QVBoxLayout *topLayout;
        QStackedLayout *bottomLayout;

        PokerDisplayWidget *pokers[4];
        std::vector<rational> operands;

        static constexpr int timeLimit = 10, timeWarningLimit = 5;
        static constexpr int correctReward = 100, correctCountReward = 20,
                incorrectPunishment = 10, abandonPunishment = 10, abandonCountPunishment = 10;

    public:
        InterfaceTimedGame(QWidget *parent = nullptr)
                : QWidget(parent), currentScore(0), highScore(0), timeLeft(0) {

            timer = new QTimer(this);
            timer->setInterval(1000);
            connect(timer, &QTimer::timeout, this, &InterfaceTimedGame::updateTime);


            topLayout = new QVBoxLayout();

            QHBoxLayout *topLayoutH = new QHBoxLayout;
            timerIconLabel = new QLabel;
            timerIconLabel->setPixmap(QIcon("img/icon_3.png").pixmap(28, 28));
            timerIconLabel->setStyleSheet(Style::sheet("scoreboard"));
            topLayoutH->addWidget(timerIconLabel);
            timerLabel = new QLabel("", this);
            timerLabel->setStyleSheet(Style::sheet("scoreboard"));
            scoreLabel = new QLabel("", this);
            scoreLabel->setStyleSheet(Style::sheet("scoreboard"));
            highScoreLabel = new QLabel("", this);
            highScoreLabel->setStyleSheet(Style::sheet("scoreboard"));
            topLayoutH->addWidget(timerLabel);
            topLayoutH->addStretch();
            topLayoutH->addWidget(scoreLabel);
            topLayout->addLayout(topLayoutH);
            topLayout->addWidget(highScoreLabel, 0, Qt::AlignRight);
            topLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));


            QHBoxLayout *pokerLayout = new QHBoxLayout();
            pokerLayout->setAlignment(Qt::AlignCenter);

            for (auto &poker: pokers) {
                poker = new PokerDisplayWidget(this);
                poker->lockEdit();
                pokerLayout->addWidget(poker);
            }


            bottomLayout = new QStackedLayout();

            { // bottom layout on start
                QVBoxLayout *bottomLayoutOnStart = new QVBoxLayout();
                bottomLayoutOnStart->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

                scoreResultLabel = new QLabel("", this);
                scoreResultLabel->setStyleSheet("font-size: 14px;");
                bottomLayoutOnStart->addWidget(scoreResultLabel, 0, Qt::AlignCenter);

                bottomLayoutOnStart->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

                startButton = new QPushButton("开始游戏", this);
                startButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                startButton->setStyleSheet(Style::sheet("button_blue"));
                connect(startButton, &QPushButton::clicked, this, &InterfaceTimedGame::startGame);
                bottomLayoutOnStart->addWidget(startButton, 0, Qt::AlignCenter);

                QWidget *bottomLayoutOnStartWidget = new QWidget(this);
                bottomLayoutOnStartWidget->setLayout(bottomLayoutOnStart);
                bottomLayout->addWidget(bottomLayoutOnStartWidget);
            }

            { // bottom layout in game
                QVBoxLayout *bottomLayoutInGame = new QVBoxLayout();
                bottomLayoutInGame->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

                scoreChangeLabel = new QLabel("", this);
                scoreChangeLabel->setStyleSheet("font-size: 14px;");
                bottomLayoutInGame->addWidget(scoreChangeLabel, 0, Qt::AlignCenter);

                answerInput = new QLineEdit(this);
                answerInput->setPlaceholderText("输入答案表达式...");
                answerInput->setStyleSheet(Style::sheet("edit_box"));
                connect(answerInput, &QLineEdit::returnPressed, this, &InterfaceTimedGame::submitAnswer);

                answerCheckIcon = new QLabel(this);
                answerCheckIcon->setPixmap(QIcon("img/icon_answer_waiting.png").pixmap(32, 32));

                QHBoxLayout *answerLayout = new QHBoxLayout();
                answerLayout->addWidget(answerInput);
                answerLayout->addWidget(answerCheckIcon);
                bottomLayoutInGame->addLayout(answerLayout);

                bottomLayoutInGame->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

                abandonButton = new QPushButton("放弃题目", this);
                abandonButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                abandonButton->setStyleSheet(Style::sheet("button_white"));
                connect(abandonButton, &QPushButton::clicked, this, &InterfaceTimedGame::refreshProblemClick);

                submitButton = new QPushButton("提交答案", this);
                submitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                submitButton->setStyleSheet(Style::sheet("button_blue"));
                connect(submitButton, &QPushButton::clicked, this, &InterfaceTimedGame::submitAnswer);

                QHBoxLayout *buttonLayout = new QHBoxLayout();
                buttonLayout->addWidget(abandonButton);
                buttonLayout->addItem(new QSpacerItem(30, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
                buttonLayout->addWidget(submitButton);
                buttonLayout->setAlignment(Qt::AlignCenter);
                bottomLayoutInGame->addLayout(buttonLayout);

                QWidget *bottomLayoutInGameWidget = new QWidget(this);
                bottomLayoutInGameWidget->setLayout(bottomLayoutInGame);
                bottomLayout->addWidget(bottomLayoutInGameWidget);
            }

            bottomLayout->setCurrentIndex(0);

            QVBoxLayout *mainLayout = new QVBoxLayout();
            mainLayout->addLayout(topLayout);
            mainLayout->addLayout(pokerLayout);
            mainLayout->addLayout(bottomLayout);
            setLayout(mainLayout);

            updateTimeLabel();
            updateScoreLabel();
        }

    private slots:

        void startGame() {
            currentScore = 0;
            timeLeft = timeLimit;
            correctCount = 0;
            abandonCount = 0;

            updateTimeLabel();
            updateScoreLabel();
            scoreChangeLabel->setText("");

            refreshProblem();

            answerCheckIcon->setPixmap(QIcon("img/icon_answer_waiting.png").pixmap(32, 32));
            bottomLayout->setCurrentIndex(1);
            timer->start();
        }

        void updateTime() {
            timeLeft--;
            updateTimeLabel();
            if (timeLeft == timeWarningLimit) {
                timerLabel->setStyleSheet(Style::sheet("scoreboard_warning"));
                timerIconLabel->setStyleSheet(Style::sheet("scoreboard_warning"));
            }
            if (timeLeft == 0) {
                timerLabel->setStyleSheet(Style::sheet("scoreboard"));
                timerIconLabel->setStyleSheet(Style::sheet("scoreboard"));
                endGame();
            }
        }

        void endGame() {
            timer->stop();
            if (currentScore > highScore) {
                highScore = currentScore;
                scoreResultLabel->setText(QString("新纪录!  分数: %1").arg(currentScore));
            }
            else
                scoreResultLabel->setText(QString("游戏结束  分数: %1").arg(currentScore));
            updateScoreLabel();
            bottomLayout->setCurrentIndex(0);
        }

        void submitAnswer() {
            std::string str = answerInput->text().toStdString();
            expression<rational> expr(((expression<int>) (str)));
            /** expression<rational> cannot be converted directly from strings (rational doesn't provide istream >> operator),
                expression<int>::value() will lose remainder in division */

            bool correct_flag = false;

            if (!expr.bad()) {
                std::vector<rational> answer_operands = expr.extract_operands();
                std::vector<rational> problem_operands = operands;
                if (answer_operands.size() == 4) {
                    bool operand_same_flag = true;
                    for (auto &i: answer_operands)
                        if (i < (rational) 0)
                            i = -i;
                    std::sort(problem_operands.begin(), problem_operands.end());
                    std::sort(answer_operands.begin(), answer_operands.end());
                    for (int i = 0; i < 4; ++i) {
                        if (answer_operands[i] != problem_operands[i]) {
                            operand_same_flag = false;
                            break;
                        }
                    }
                    if (operand_same_flag)
                        if (expr.value() == (rational) 24)
                            correct_flag = true; // requires: operands are the same && value == 24
                }

                if (correct_flag) {
                    currentScore += correctReward + correctCount * correctCountReward;
                    if (correctCount == 0)
                        scoreChangeLabel->setText(
                                QString("答对了!  +%1").arg(correctReward + correctCount * correctCountReward));
                    else
                        scoreChangeLabel->setText(QString("连续答对 %1 题!  +%2").arg(correctCount + 1).arg(
                                correctReward + correctCount * correctCountReward));
                    ++correctCount;
                    abandonCount = 0;
                    answerCheckIcon->setPixmap(QIcon("img/icon_answer_correct.png").pixmap(32, 32));
                    refreshProblem();
                }
                else {
                    currentScore -= incorrectPunishment;
                    if (currentScore < 0)
                        currentScore = 0;
                    scoreChangeLabel->setText(QString("答错了!  -%1").arg(incorrectPunishment));
                    correctCount = 0;
                    answerCheckIcon->setPixmap(QIcon("img/icon_answer_incorrect.png").pixmap(32, 32));
                }
            }
            else {
                scoreChangeLabel->setText("表达式非法, 请检查输入");
                answerCheckIcon->setPixmap(QIcon("img/icon_answer_waiting.png").pixmap(32, 32));
            }
            updateScoreLabel();
        }

        void refreshProblemClick() {
            currentScore -= abandonPunishment + abandonCount * abandonCountPunishment;
            if (currentScore < 0)
                currentScore = 0;
            if (abandonCount == 0)
                scoreChangeLabel->setText(
                        QString("放弃题目  -%1").arg(abandonPunishment + abandonCount * abandonCountPunishment));
            else
                scoreChangeLabel->setText(QString("连续放弃 %1 题  -%2").arg(abandonCount + 1).arg(
                        abandonPunishment + abandonCount * abandonCountPunishment));
            ++abandonCount;
            correctCount = 0;
            answerCheckIcon->setPixmap(QIcon("img/icon_answer_waiting.png").pixmap(32, 32));
            updateScoreLabel();
            refreshProblem();
        }

    private:

        void updateTimeLabel() {
            int minutes = timeLeft / 60;
            int seconds = timeLeft % 60;
            timerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        }

        void updateScoreLabel() {
            scoreLabel->setText(QString("%1").arg(currentScore, 6, 10, QChar('0')));
            highScoreLabel->setText(QString("HI %1").arg(highScore, 6, 10, QChar('0')));
            abandonButton->setText(
                    QString("放弃题目 (-%1)").arg(abandonPunishment + abandonCount * abandonCountPunishment));
        }

        void refreshProblem() {
            operands = randomized_integers(4, 1, 13, 24);
            for (int i = 0; i < 4; ++i)
                pokers[i]->setValue((int) operands[i]);
            answerInput->setText("");
        }
    };


}

#endif
