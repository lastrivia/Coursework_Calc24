#ifndef CALC_INTERFACE_MENU_H
#define CALC_INTERFACE_MENU_H

#include <QListWidget>

namespace calc {


    QColor darkenColor(const QColor &color, int factor);

    class MenuWidget : public QListWidget {
    Q_OBJECT

    public:
        MenuWidget(QWidget *parent = nullptr) : QListWidget(parent) {
            setIconSize(QSize(24, 24)); // 设置图标大小

            // 创建每个菜单项的名称和对应的图标路径
            QStringList items = {"基本计算", "文件处理", "计时挑战", "联网比赛", "设置"};
            QStringList icons = {"img/icon_1.png", "img/icon_2.png", "img/icon_3.png", "img/icon_4.png",
                                 "img/icon_5.png"};

            // 遍历列表，创建每个项并设置图标
            for (int i = 0; i < items.size(); ++i) {
                QListWidgetItem *item = new QListWidgetItem(items[i]);
                item->setIcon(QIcon(icons[i]));  // 设置图标
                addItem(item);  // 添加到列表

                // 设置字体大小
                QFont font;
                font.setPointSize(11);
                item->setFont(font);
            }


            setFocusPolicy(Qt::NoFocus);
            setCurrentRow(0);

            // 获取当前系统默认背景色
            QPalette palette = this->palette();
            QColor defaultColor = palette.color(QPalette::Window);  // 获取默认背景色

            // 调暗颜色（lighten）和进一步调暗颜色（darker）
            QColor hoverColor = darkenColor(defaultColor, 20);  // 稍微调暗
            QColor selectedColor = darkenColor(defaultColor, 30);  // 更加调暗

            setStyleSheet(QString(
                    "QListWidget {"
                    "  border: none;"
                    "  background-color: %1;"
                    "}"
                    "QListWidget::item {"
                    "  border-radius: 10px;"
                    "  padding: 10px;"
                    "  background-color: #f0f0f0;"
                    "  margin-bottom: 5px;"  // 可选，增加项之间的间距
                    "  color: black;"
                    "}"
                    "QListWidget::item:hover {"
                    "  background-color: %2;"
                    "  color: #2a2a2a;"
                    "}"
                    "QListWidget::item:selected {"
                    "  background-color: %3;"
                    "  border: none;"
                    "}"
                    "QListWidget::item:selected:active {"
                    "  outline: none;"
                    "}"
            ).arg(defaultColor.name(), hoverColor.name(), selectedColor.name()));
        }

    };

}

#endif
