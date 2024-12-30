#include <QApplication>
#include "interface.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    calc::MainWindow window;
    window.show();

    return QApplication::exec();
}
