#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "serialworker.h"
#include "serial/serial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void serialLog(const QString &text);

private slots:
    void on_serialBtn_clicked();

private:
    Ui::MainWindow *_ui;
    serial::Serial _serial;

    SerialWorker _serialWorker;
};

#endif // MAINWINDOW_H
