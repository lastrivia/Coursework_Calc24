#ifndef CALC_INTERFACE_FILE_H
#define CALC_INTERFACE_FILE_H

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

#include "solver.h"
#include "poker.h"

namespace calc {

    class PageFile : public QWidget {
    Q_OBJECT

    private:
        QLineEdit *filePathInput;
        QLabel *resultLabel;
        QPushButton *openFileButton, *calculateButton, *saveButton;

        std::vector<std::vector<rational> > file_operands;
        std::vector<int> file_results;
        std::vector<std::string> file_result_strings; // TODO unused yet

    public:
        PageFile(QWidget *parent = nullptr) : QWidget(parent) {

            QHBoxLayout *fileLayout = new QHBoxLayout;

            QLabel *iconLabel = new QLabel;
            iconLabel->setPixmap(QIcon("img/icon_2.png").pixmap(32, 32)); // 替换为实际图标路径
            fileLayout->addWidget(iconLabel);

            fileLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            filePathInput = new QLineEdit;
            filePathInput->setStyleSheet(
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
            fileLayout->addWidget(filePathInput);

            fileLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            openFileButton = new QPushButton("选择文件");
            openFileButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            openFileButton->setStyleSheet(
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
            QFont font;
            font.setPointSize(11);
            openFileButton->setFont(font);
            connect(openFileButton, &QPushButton::clicked, this, &PageFile::openFile);
            fileLayout->addWidget(openFileButton);


            QHBoxLayout *operationLayout = new QHBoxLayout;

            calculateButton = new QPushButton("执行运算");
            calculateButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            calculateButton->setStyleSheet(
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
            connect(calculateButton, &QPushButton::clicked, this, &PageFile::performCalculation);
            operationLayout->addWidget(calculateButton);

            operationLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            resultLabel = new QLabel("等待中...");
            operationLayout->addWidget(resultLabel);

            operationLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

            saveButton = new QPushButton("保存结果");
            saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            saveButton->setEnabled(false);  // 初始状态为不可点击
            saveButton->setStyleSheet(
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
            connect(saveButton, &QPushButton::clicked, this, &PageFile::saveFile);
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
                    ss << "已完成 " << file_operands.size() << " 组计算，点击导出结果 ->";
                    resultLabel->setText(ss.str().c_str());
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
                    // fout << '\t' << file_result_strings[i];
                    fout << '\n';
                }

                QProcess::startDetached("notepad.exe", QStringList() << savePath);
            }
            else QMessageBox::warning(this, "保存失败", "错误：文件拒绝访问");
        }
    };

}

#endif
