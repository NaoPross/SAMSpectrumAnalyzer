#include "serial/serial.h"

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    serial::Serial("/dev/ttyUSB0");

    return a.exec();
}
