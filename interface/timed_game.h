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
#include "../algorithm/random.h"
#include "poker_display.h"
#include "settings.h"

namespace calc {

    class InterfaceTimedGame : public QWidget {
    Q_OBJECT

        int currentScore;
        SETTING_SAVED_DATA int highScore;
        int timeLeft;
        int correctCount, abandonCount;

        bool cheatFlag;

        QTimer *gameTimer, *blinkTimer;
        QLabel *timerIconLabel, *timerLabel, *scoreLabel, *highScoreLabel;
        QLineEdit *answerInput;
        QLabel *answerCheckIcon;
        QLabel *scoreResultLabel, *scoreChangeLabel;
        QPushButton *startButton, *submitButton, *abandonButton;
        QVBoxLayout *topLayout;
        QStackedLayout *bottomLayout;

        PokerDisplayWidget *pokers[4];
        std::vector<rational> operands;

        SETTING_OPTION_DATA int timeLimit;
        static constexpr int timeWarningLimit = 20;
        static constexpr int correctReward = 100, correctCountReward = 20,
                incorrectPunishment = 10, abandonPunishment = 10, abandonCountPunishment = 10;

    public:
        friend class InterfaceSettings;

        InterfaceTimedGame(QWidget *parent = nullptr)
                : QWidget(parent), currentScore(0), highScore(0), timeLeft(0) {

            gameTimer = new QTimer(this);
            gameTimer->setInterval(1000);
            connect(gameTimer, &QTimer::timeout, this, &InterfaceTimedGame::updateTime);

            blinkTimer = new QTimer(this);
            blinkTimer->setInterval(500);
            connect(blinkTimer, &QTimer::timeout, this, &InterfaceTimedGame::blinkTime);


            topLayout = new QVBoxLayout();

            QHBoxLayout *topLayoutH = new QHBoxLayout;
            timerIconLabel = new QLabel;
            timerIconLabel->setPixmap(QIcon("img/icon_menu_clock.png").pixmap(28, 28));
            timerIconLabel->setStyleSheet(Style::QSSKey("scoreboard"));
            topLayoutH->addWidget(timerIconLabel);
            timerLabel = new QLabel("", this);
            timerLabel->setStyleSheet(Style::QSSKey("scoreboard"));
            scoreLabel = new QLabel("", this);
            scoreLabel->setStyleSheet(Style::QSSKey("scoreboard"));
            highScoreLabel = new QLabel("", this);
            highScoreLabel->setStyleSheet(Style::QSSKey("scoreboard"));
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
                startButton->setStyleSheet(Style::QSSKey("button_blue"));
                startButton->setFixedWidth(180);
                startButton->setIcon(QIcon("img/icon_button_start_w.png"));
                startButton->setIconSize(QSize(20, 20));
                startButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
                answerInput->setStyleSheet(Style::QSSKey("edit_box"));
                connect(answerInput, &QLineEdit::returnPressed, this, &InterfaceTimedGame::submitAnswer);

                answerCheckIcon = new QLabel(this);
                answerCheckIcon->setPixmap(QIcon("img/icon_answer_waiting.png").pixmap(32, 32));

                QHBoxLayout *answerLayout = new QHBoxLayout();
                answerLayout->addWidget(answerInput);
                answerLayout->addWidget(answerCheckIcon);
                bottomLayoutInGame->addLayout(answerLayout);

                bottomLayoutInGame->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

                abandonButton = new QPushButton("放弃题目", this);
                abandonButton->setStyleSheet(Style::QSSKey("button_white"));
                abandonButton->setFixedWidth(180);
                abandonButton->setIcon(QIcon("img/icon_button_refresh.png"));
                abandonButton->setIconSize(QSize(20, 20));
                abandonButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                connect(abandonButton, &QPushButton::clicked, this, &InterfaceTimedGame::refreshProblemClick);

                submitButton = new QPushButton("提交答案", this);
                submitButton->setStyleSheet(Style::QSSKey("button_blue"));
                submitButton->setFixedWidth(180);
                submitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
            gameTimer->start();
        }

        void updateTime() {
            timeLeft--;
            updateTimeLabel();
            if (timeLeft == 0) {
                timerLabel->setVisible(true);
                endGame();
            }
            else if (timeLeft <= timeWarningLimit) {
                timerLabel->setVisible(true);
                blinkTimer->start();
            }
        }

        void blinkTime() {
            timerLabel->setVisible(false);
            blinkTimer->stop();
        }

        void endGame() {
            gameTimer->stop();
            if (currentScore > highScore) {
                highScore = currentScore;
                scoreResultLabel->setText(QString("新纪录！ 分数: %1").arg(currentScore));
            }
            else
                scoreResultLabel->setText(QString("游戏结束  分数: %1").arg(currentScore));
            updateScoreLabel();
            bottomLayout->setCurrentIndex(0);
        }

        void submitAnswer() {
            std::string str = answerInput->text().toStdString();
            expression<rational> expr(((expression<poker_int>) (str)));
            // expression<rational> cannot be converted directly from strings (rational doesn't provide istream >> operator)

            bool correct_flag = false;

            if (!expr.bad()) {
                std::vector<rational> answer_operands = expr.extract_operands();
                std::vector<rational> problem_operands = operands;
                for (auto &i: answer_operands)
                    if (i < (rational) 0)
                        i = -i;
                if (same_elements(answer_operands, problem_operands) && expr.value() == (rational) 24)
                    correct_flag = true; // requires: operands are the same && value == 24

                if (correct_flag) {
                    if (cheatFlag) {
                        timeLeft = 0;
                        currentScore = 0;
                        endGame();
                        scoreResultLabel->setText(QString("不许作弊哦！ 游戏结束  分数: 0"));
                    }
                    else {
                        currentScore += correctReward + correctCount * correctCountReward;
                        if (correctCount == 0)
                            scoreChangeLabel->setText(
                                    QString("答对了！ +%1").arg(correctReward + correctCount * correctCountReward));
                        else
                            scoreChangeLabel->setText(QString("连续答对 %1 题！ +%2").arg(correctCount + 1).arg(
                                    correctReward + correctCount * correctCountReward));
                        ++correctCount;
                        abandonCount = 0;
                        answerCheckIcon->setPixmap(QIcon("img/icon_answer_correct.png").pixmap(32, 32));
                        refreshProblem();
                    }
                }
                else {
                    currentScore -= incorrectPunishment;
                    if (currentScore < 0)
                        currentScore = 0;
                    scoreChangeLabel->setText(QString("答错了！ -%1").arg(incorrectPunishment));
                    correctCount = 0;
                    answerCheckIcon->setPixmap(QIcon("img/icon_answer_incorrect.png").pixmap(32, 32));
                }
            }
            else {
                scoreChangeLabel->setText("表达式非法，请检查输入");
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
            cheatFlag = false;
        }

    public:

        bool gameStarted() const {
            return timeLeft > 0;
        }

        const std::vector<rational> &getOperands() const {
            return operands;
        }

        void setCheatFlag() {
            cheatFlag = true;
        }
    };


}

#endif
