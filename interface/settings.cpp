#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QValidator>
#include <QLineEdit>

#include <fstream>

#include "settings.h"
#include "file_calculator.h"
#include "timed_game.h"

namespace calc {

    SettingWidgetInt::SettingWidgetInt(int minValue, int defaultValue, int maxValue, int *boundValue,
                                       const QString &text, QWidget *parent) :
            SettingWidget(parent), boundValueLink(boundValue), defaultValue(defaultValue) {

        QLabel *label = new QLabel(text, this);
        label->setStyleSheet("font-size: 14px;");

        lineEdit = new QLineEdit(parent);
        lineEdit->setStyleSheet(Style::QSSKey("edit_box"));
        lineEdit->setFixedWidth(60);

        lineEdit->setText(QString("%1").arg(*boundValue));
        connect(lineEdit, &QLineEdit::editingFinished, [this, boundValue, minValue, defaultValue, maxValue]() {
            bool ok;
            int x = lineEdit->text().toInt(&ok);

            if (!ok) {
                lineEdit->blockSignals(true);
                lineEdit->setText(QString("%1").arg(defaultValue));
                lineEdit->blockSignals(false);
                *boundValue = defaultValue;
            }
            else if (x < minValue) {
                lineEdit->blockSignals(true);
                lineEdit->setText(QString("%1").arg(minValue));
                lineEdit->blockSignals(false);
                *boundValue = minValue;
            }
            else if (x > maxValue) {
                lineEdit->blockSignals(true);
                lineEdit->setText(QString("%1").arg(maxValue));
                lineEdit->blockSignals(false);
                *boundValue = maxValue;
            }
            else
                *boundValue = x;
        });

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addSpacerItem(new QSpacerItem(0, 36, QSizePolicy::Minimum, QSizePolicy::Fixed));
        mainLayout->addWidget(label);
        mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        mainLayout->addWidget(lineEdit);
    }

    SettingWidgetBool::SettingWidgetBool(bool *boundValue, bool defaultValue, const QString &text, QWidget *parent) :
            SettingWidget(parent), boundValueLink(boundValue), defaultValue(defaultValue) {

        QLabel *label = new QLabel(text, this);
        label->setStyleSheet("font-size: 14px;");
        statusLabel = new QLabel("", this);
        statusLabel->setStyleSheet("font-size: 14px;");

        checkBox = new QCheckBox(this);

        checkBox->setChecked(*boundValue);
        checkBox->setStyleSheet(Style::QSSKey("switch_checkbox"));
        setLabel();

        connect(checkBox, &QCheckBox::toggled, this, [this, boundValue](bool checked) {
            *boundValue = checked;
            setLabel();
        });

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addSpacerItem(new QSpacerItem(0, 36, QSizePolicy::Minimum, QSizePolicy::Fixed));
        mainLayout->addWidget(label);
        mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        mainLayout->addWidget(statusLabel);
        mainLayout->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
        mainLayout->addWidget(checkBox);

    }

    InterfaceSettings::InterfaceSettings(InterfaceFileCalculator *fileCalculator,
                                         InterfaceTimedGame *timedGame,
                                         QWidget *parent) :
            QWidget(parent),
            fileCalculatorWidgetLink(fileCalculator),
            timedGameWidgetLink(timedGame) {


        fileCalculatorWidgetLink->maxThreads = 1;
        fileCalculatorWidgetLink->outputFileStrings = false;
        timedGameWidgetLink->highScore = 0;
        timedGameWidgetLink->timeLimit = 120;

        std::ifstream fin("settings");

        if (fin.is_open()) {
            std::string key;
            while (std::getline(fin, key, ':')) {
                if (key == "fileCalculator.maxThreads")
                    fin >> fileCalculatorWidgetLink->maxThreads;
                else if (key == "fileCalculator.outputFileStrings")
                    fin >> fileCalculatorWidgetLink->outputFileStrings;
                else if (key == "timedGame.highScore")
                    fin >> timedGameWidgetLink->highScore;
                else if (key == "timedGame.timeLimit")
                    fin >> timedGameWidgetLink->timeLimit;
                while (fin.peek() == ' ' || fin.peek() == '\n')
                    fin.ignore();
            }
        }

        settingItem[0] = new SettingWidgetInt(
                1,
                1,
                32,
                &fileCalculatorWidgetLink->maxThreads,
                "文件处理线程数 (TODO)",
                this
        );
        settingItem[1] = new SettingWidgetBool(
                &fileCalculatorWidgetLink->outputFileStrings,
                false,
                "文件输出完整表达式",
                this
        );
        settingItem[2] = new SettingWidgetInt(
                20,
                120,
                300,
                &timedGameWidgetLink->timeLimit,
                "计时挑战时间限制",
                this
        );

        QVBoxLayout *settingsLayout = new QVBoxLayout(this);

        for (int i = 0; i < settingItemsCount; ++i) {
            if (i) {
                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                //line->setFrameShadow(QFrame::Sunken);
                line->setStyleSheet("color: rgba(0, 0, 0, 0.1);");
                line->setFixedHeight(1);
                settingsLayout->addWidget(line);
            }
            settingsLayout->addWidget(settingItem[i]);
        }

        resetButton = new QPushButton("恢复默认", this);
        resetButton->setStyleSheet(Style::QSSKey("button_white"));
        resetButton->setIcon(QIcon("img/icon_button_refresh.png"));
        resetButton->setIconSize(QSize(20, 20));
        resetButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(resetButton, &QPushButton::clicked, this, &InterfaceSettings::resetClick);

        settingsLayout->addSpacerItem(new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Fixed));
        settingsLayout->addWidget(resetButton, 0, Qt::AlignCenter);

        settingsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        setLayout(settingsLayout);
    }

    InterfaceSettings::~InterfaceSettings() {
        std::ofstream fout("settings");
        if (fileCalculatorWidgetLink) {
            fout << "fileCalculator.maxThreads: " << fileCalculatorWidgetLink->maxThreads << "\n";
            fout << "fileCalculator.outputFileStrings: " << fileCalculatorWidgetLink->outputFileStrings << "\n";
        }
        if (timedGameWidgetLink) {
            fout << "timedGame.highScore: " << timedGameWidgetLink->highScore << "\n";
            fout << "timedGame.timeLimit: " << timedGameWidgetLink->timeLimit << "\n";
        }
    }

    void InterfaceSettings::resetClick() {
        for (auto i: settingItem)
            i->reset();
    }
}
