#ifndef CALC_SETTINGS_H
#define CALC_SETTINGS_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

#define SETTING_OPTION_DATA
#define SETTING_SAVED_DATA

namespace calc {

    class InterfaceFileCalculator;

    class InterfaceTimedGame;

    class SettingWidget : public QWidget {
    Q_OBJECT

        friend class InterfaceSettings;

    protected:
        SettingWidget(QWidget *parent = nullptr) : QWidget(parent) {}

        virtual void reset() = 0;
    };


    class InterfaceSettings : public QWidget {
    Q_OBJECT

    private:

        InterfaceFileCalculator *fileCalculatorWidgetLink;
        InterfaceTimedGame *timedGameWidgetLink;

        static constexpr int settingItemsCount = 3;
        SettingWidget *settingItem[settingItemsCount];
        QPushButton *resetButton, *gitHubButton;

    public:
        InterfaceSettings(InterfaceFileCalculator *fileCalculator,
                          InterfaceTimedGame *timedGame,
                          QWidget *parent = nullptr);

        ~InterfaceSettings() override;

    private slots:

        void resetClick();
    };


    class SettingWidgetInt : public SettingWidget {
    Q_OBJECT

    private:
        QLineEdit *lineEdit;
        int *boundValueLink;
        int defaultValue;

        friend class InterfaceSettings;

        SettingWidgetInt(int minValue, int defaultValue, int maxValue, int *boundValue, const QString &text,
                         QWidget *parent = nullptr);

        void reset() override {
            lineEdit->setText(QString("%1").arg(defaultValue));
            emit lineEdit->editingFinished();
        }
    };


    class SettingWidgetBool : public SettingWidget {
    Q_OBJECT

    private:
        QCheckBox *checkBox;
        bool *boundValueLink;
        bool defaultValue;
        QLabel *statusLabel;

        friend class InterfaceSettings;

        SettingWidgetBool(bool *boundValue, bool defaultValue, const QString &text,
                          QWidget *parent = nullptr);

        void reset() override {
            checkBox->setChecked(defaultValue);
        };

        void setLabel() {
            if(checkBox->isChecked())
                statusLabel->setText("开");
            else
                statusLabel->setText("关");
        }
    };
}

#endif
