#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QVector>

#include <complex>

#include "qcustomplot.h"
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
    bool openSerialDevice();
    void closeSerialDevice();

    void on_serialBtn_clicked();
    void on_adjustAxisCheckBox_toggled(bool checked);
    void on_logFreqAxisCheckBox_toggled(bool checked);
    void on_plotTypeSelCombo_currentIndexChanged(int index);

    void on_actionQuit_triggered();
    void on_actionSave_data_triggered();
    void on_actionExport_image_triggered();


private:
    Ui::MainWindow *_ui;
    serial::Serial _serial;

    SerialWorker _serialWorker;
    QVector<double> _xsamples;
    QVector<double> _ysamples;

    QSharedPointer<QCPAxisTicker> _defaultTicker;
    QSharedPointer<QCPAxisTicker> _logTicker;
};

#endif // MAINWINDOW_H
