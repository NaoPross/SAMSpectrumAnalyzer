#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <string>

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
    void readSerialLog();
    void on_serialBtn_clicked();

private:
    Ui::MainWindow *_ui;
    QTimer *_timer;

    serial::Serial *_serialDevice = nullptr;
};

#endif // MAINWINDOW_H
