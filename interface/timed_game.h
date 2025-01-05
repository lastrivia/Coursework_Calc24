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

namespace calc {

    class InterfaceTimedGame : public QWidget {
    Q_OBJECT

        int currentScore, highScore;
        int timeLeft;

        QTimer *timer;
        QLabel *timerLabel, *scoreLabel, *highScoreLabel;
        QLineEdit *answerInput;
        QLabel *answerCheckIcon;
        QLabel *scoreResultLabel;
        QPushButton *startButton, *submitButton, *refreshButton;
        QVBoxLayout *topLayout;
        QStackedLayout *bottomLayout;

        PokerDisplayWidget *pokers[4];
        std::vector<rational> operands;

    public:
        InterfaceTimedGame(QWidget *parent = nullptr)
                : QWidget(parent), currentScore(0), highScore(0), timeLeft(10) {

            timer = new QTimer(this);
            timer->setInterval(1000);
            connect(timer, &QTimer::timeout, this, &InterfaceTimedGame::updateTime);


            topLayout = new QVBoxLayout();

            QHBoxLayout *topLayoutH = new QHBoxLayout;
            timerLabel = new QLabel("", this);
            timerLabel->setStyleSheet("font-size: 16px;");
            scoreLabel = new QLabel("", this);
            scoreLabel->setStyleSheet("font-size: 16px;");
            highScoreLabel = new QLabel("", this);
            highScoreLabel->setStyleSheet("font-size: 16px;");
            updateTimeLabel();
            updateScoreLabel();
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

                scoreResultLabel = new QLabel(" ", this);
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

                answerInput = new QLineEdit(this);
                answerInput->setPlaceholderText("输入答案表达式...");
                answerInput->setStyleSheet(Style::sheet("edit_box"));

                answerCheckIcon = new QLabel(this);
                answerCheckIcon->setPixmap(QPixmap(" "));

                QHBoxLayout *answerLayout = new QHBoxLayout();
                answerLayout->addWidget(answerInput);
                answerLayout->addWidget(answerCheckIcon);
                bottomLayoutInGame->addLayout(answerLayout);

                bottomLayoutInGame->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

                refreshButton = new QPushButton("刷新题目", this);
                refreshButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                refreshButton->setStyleSheet(Style::sheet("button_white"));
                connect(refreshButton, &QPushButton::clicked, this, &InterfaceTimedGame::refreshProblemClick);

                submitButton = new QPushButton("提交答案", this);
                submitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                submitButton->setStyleSheet(Style::sheet("button_blue"));
                connect(submitButton, &QPushButton::clicked, this, &InterfaceTimedGame::submitAnswer);

                QHBoxLayout *buttonLayout = new QHBoxLayout();
                buttonLayout->addWidget(refreshButton);
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
        }

    private slots:

        void startGame() {
            currentScore = 0;
            timeLeft = 10;
            timer->start();
            updateScoreLabel();
            refreshProblem();
            bottomLayout->setCurrentIndex(1);
        }

        void updateTime() {
            timeLeft--;
            updateTimeLabel();
            if (timeLeft == 0)
                endGame();
        }

        void endGame() {
            timer->stop();
            bottomLayout->setCurrentIndex(0);
        }

        void submitAnswer() {
            if (true) {  // NOLINT TODO
                currentScore++;
                refreshProblem();
            }
            if (currentScore > highScore) {
                highScore = currentScore;
            }
            updateScoreLabel();
        }

        void refreshProblemClick() {
            refreshProblem();
        }

    private:

        void updateTimeLabel() {
            int minutes = timeLeft / 60;
            int seconds = timeLeft % 60;
            timerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        }

        void updateScoreLabel() {
            scoreLabel->setText(QString::number(currentScore));
            highScoreLabel->setText("HI " + QString::number(highScore));
        }

        void refreshProblem() {
            operands = randomized_integers(4, 1, 13, 24);
            for (int i = 0; i < 4; ++i)
                pokers[i]->setValue((int) operands[i]);
        }
    };


}

#endif
