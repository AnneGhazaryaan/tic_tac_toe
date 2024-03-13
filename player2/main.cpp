#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Player 2"); // Set window title to Player 2
    w.show();
    return a.exec();
}
