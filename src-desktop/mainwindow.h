#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QVector>

#include "serialworker.h"
#include "serial/serial.h"
#include "../src-common/signals.hpp"

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
    void serialDataReceiver(QVector<sam::complex_uint16_t> data);
    void on_serialBtn_clicked();

private:
    Ui::MainWindow *_ui;
    serial::Serial _serial;

    SerialWorker _serialWorker;
    QVector<double> _xsamples;
    QVector<double> _ysamples;
};

#endif // MAINWINDOW_H
