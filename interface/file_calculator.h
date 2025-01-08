#ifndef CALC_FILE_CALCULATOR_H
#define CALC_FILE_CALCULATOR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <fstream>
#include <filesystem>
#include <QProcess>

#include "../algorithm/solver.h"
#include "../algorithm/utils.h"
#include "style.h"
#include "settings.h"

namespace calc {

    class InterfaceFileCalculator : public QWidget {
    Q_OBJECT

    private:
        QLineEdit *filePathInput;
        QLabel *resultLabel, *resultIndicatorIcon;
        QPushButton *openFileButton, *calculateButton, *saveButton;

        SETTING_OPTION_DATA int maxThreads;
        SETTING_OPTION_DATA bool outputFileStrings;
        std::vector<std::vector<rational> > file_operands;
        std::vector<int> file_results;
        std::vector<std::string> file_result_strings;

    public:
        friend class InterfaceSettings;

        InterfaceFileCalculator(QWidget *parent = nullptr) : QWidget(parent) {

            Style::QSSKey("button_white");

            QHBoxLayout *fileLayout = new QHBoxLayout;

            QLabel *iconLabel = new QLabel;
            iconLabel->setPixmap(QIcon("img/icon_menu_file.png").pixmap(32, 32));
            fileLayout->addWidget(iconLabel);

            fileLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            filePathInput = new QLineEdit;
            filePathInput->setStyleSheet(Style::QSSKey("edit_box"));
            fileLayout->addWidget(filePathInput);

            fileLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            openFileButton = new QPushButton("选择文件");
            openFileButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            openFileButton->setStyleSheet(Style::QSSKey("button_white"));
            QFont font;
            font.setPointSize(11);
            openFileButton->setFont(font);
            connect(openFileButton, &QPushButton::clicked, this, &InterfaceFileCalculator::openFile);
            fileLayout->addWidget(openFileButton);


            QHBoxLayout *operationLayout = new QHBoxLayout;

            calculateButton = new QPushButton("执行运算");
            calculateButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            calculateButton->setStyleSheet(Style::QSSKey("button_blue"));
            connect(calculateButton, &QPushButton::clicked, this, &InterfaceFileCalculator::performCalculation);
            operationLayout->addWidget(calculateButton);

            operationLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            resultLabel = new QLabel("等待中...");
            operationLayout->addWidget(resultLabel);

            resultIndicatorIcon = new QLabel(this);
            resultIndicatorIcon->setPixmap(QIcon("").pixmap(20, 20));
            operationLayout->addWidget(resultIndicatorIcon);

            operationLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

            saveButton = new QPushButton("保存结果");
            saveButton->setStyleSheet(Style::QSSKey("button_white"));
            saveButton->setIcon(QIcon("img/icon_button_save.png"));
            saveButton->setIconSize(QSize(20, 20));
            saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            saveButton->setEnabled(false);
            connect(saveButton, &QPushButton::clicked, this, &InterfaceFileCalculator::saveFile);
            operationLayout->addWidget(saveButton);


            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->addLayout(fileLayout);
            mainLayout->addLayout(operationLayout);
            setLayout(mainLayout);
        }

    private slots:

        void openFile() {
            QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件 (*.*)");
            if (!filePath.isEmpty()) {
                filePathInput->setText(filePath);
            }
        }

        void performCalculation() {

            saveButton->setEnabled(false);
            resultIndicatorIcon->setPixmap(QIcon("").pixmap(20, 20));
            std::string file_path = filePathInput->text().toStdString();

            if (std::filesystem::exists(file_path)) {
                std::ifstream fin(file_path);
                if (fin.is_open()) {

                    std::vector<rational> input_operand(4);
                    std::string in_str;

                    file_operands.clear();

                    while (true) {
                        for (int i = 0; i < 4; ++i) {
                            fin >> in_str;
                            input_operand[i] = poker_to_int(in_str);
                        }
                        if (!fin) // end of input
                            break;
                        file_operands.push_back(input_operand);
                    }
                    fin.close();

                    int n = (int) file_operands.size();
                    file_results.resize(n);
                    file_result_strings.resize(n);

                    solver solver_instance(4, 24);
                    for (int i = 0; i < n; ++i) {
                        solver_instance.set_operands(file_operands[i]);
                        file_results[i] = solver_instance.solve(file_result_strings[i]);
                    }

                    std::stringstream ss;
                    ss << "已完成 " << file_operands.size() << " 组计算，点击导出结果";
                    resultLabel->setText(ss.str().c_str());
                    resultIndicatorIcon->setPixmap(QIcon("img/icon_button_start.png").pixmap(20, 20));
                    saveButton->setEnabled(true);
                }
                else resultLabel->setText("错误：文件拒绝访问");
            }
            else resultLabel->setText("错误：文件不存在");
        }

        void saveFile() {
            QString savePath = QFileDialog::getSaveFileName(this, "保存结果", "", "文本文件 (*.txt);;所有文件 (*.*)");
            std::string save_path = savePath.toStdString();
            std::ofstream fout(save_path);

            if (fout.is_open()) {
                int n = (int) file_operands.size();

                for (int i = 0; i < n; ++i) {
                    if (file_results[i])
                        fout << '+';
                    else fout << '-';
                    for (int j = 0; j < 4; ++j)
                        fout << '\t' << int_to_poker((int) file_operands[i][j]);
                    if (outputFileStrings)
                        fout << '\t' << file_result_strings[i];
                    fout << '\n';
                }

                QProcess::startDetached("notepad.exe", QStringList() << savePath);
            }
            else QMessageBox::warning(this, "保存失败", "错误：文件拒绝访问");
        }
    };

}

#endif
