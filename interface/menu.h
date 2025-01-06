#ifndef CALC_MENU_H
#define CALC_MENU_H

#include <QListWidget>

#include "style.h"

namespace calc {

    class MenuWidget : public QListWidget {
    Q_OBJECT

    public:
        MenuWidget(QWidget *parent = nullptr) : QListWidget(parent) {
            setIconSize(QSize(24, 24));

            QStringList items = {"基本计算", "文件处理", "计时挑战", "联网比赛", "设置"};
            QStringList icons = {"img/icon_menu_calculator.png", "img/icon_menu_file.png", "img/icon_menu_clock.png", "img/icon_menu_battle.png",
                                 "img/icon_menu_settings.png"};

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
