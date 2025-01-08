#include <QApplication>
#include <iostream>
#include "interface/interface.h"

int main(int argc, char *argv[]) {

    std::ios::sync_with_stdio(false);

    QApplication app(argc, argv);

    calc::MainWindow window;
    window.show();

    return QApplication::exec();
}
