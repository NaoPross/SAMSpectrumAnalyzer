#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QVector>

#include <complex>

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
    void serialDataReceiver(QVector<std::complex<int>> data);

    void on_serialBtn_clicked();
    void on_adjustAxisCheckBox_toggled(bool value);
    void on_plotTypeSelCombo_currentIndexChanged(int index);

private:
    Ui::MainWindow *_ui;
    serial::Serial _serial;

    SerialWorker _serialWorker;
    QVector<double> _xsamples;
    QVector<double> _ysamples;
};

#endif // MAINWINDOW_H
