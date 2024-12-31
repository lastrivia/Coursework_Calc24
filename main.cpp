#include <QApplication>
#include "interface/interface.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    calc::MainWindow window;
    window.show();

    return QApplication::exec();
}
