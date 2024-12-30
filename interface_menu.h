#ifndef CALC_INTERFACE_MENU_H
#define CALC_INTERFACE_MENU_H

#include <QListWidget>

#include "style.h"

namespace calc {

    class MenuWidget : public QListWidget {
    Q_OBJECT

    public:
        MenuWidget(QWidget *parent = nullptr) : QListWidget(parent) {
            setIconSize(QSize(24, 24));

            QStringList items = {"基本计算", "文件处理", "计时挑战", "联网比赛", "设置"};
            QStringList icons = {"img/icon_1.png", "img/icon_2.png", "img/icon_3.png", "img/icon_4.png",
                                 "img/icon_5.png"};

            for (int i = 0; i < items.size(); ++i) {
                QListWidgetItem *item = new QListWidgetItem(items[i]);
                item->setIcon(QIcon(icons[i]));
                addItem(item);

                QFont font;
                font.setPointSize(11);
                item->setFont(font);
            }

            setFocusPolicy(Qt::NoFocus);
            setCurrentRow(0);

            setStyleSheet(Style::sheet("menu_list"));
        }

    };

}

#endif
