#include "mainwindow.h"
#include <QTcpSocket>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication cardController(argc, argv);

    MainWindow w;
    w.show();

    return cardController.exec();
}
