#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaType>

#include <exception>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _serial(),
    _serialWorker(_serial)
{
    _ui->setupUi(this);
    _ui->serialDevice->setText(
#ifdef _WIN32
        "COM1"
#elif defined(__linux__)
        "/dev/ttyUSB0"
#endif
    );

    // graph 0 on bottom and left axis
    _ui->plot->addGraph(_ui->plot->xAxis, _ui->plot->yAxis);
    // graph 1 on top and right axis
    _ui->plot->addGraph(_ui->plot->xAxis2, _ui->plot->yAxis2);

    // main axis
    _ui->plot->xAxis->setLabel("Frequency");
    _ui->plot->yAxis->setLabel("Amplitude");

    _ui->plot->xAxis->setRange(0, 10000);
    _ui->plot->yAxis->setRange(-5, 5);

    // secondary axis
    _ui->plot->xAxis2->setLabel("");
    _ui->plot->yAxis2->setLabel("");

    // set seconday axis to log
    // QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    // _ui->plot->xAxis2->setTicker(logTicker);
    // _ui->plot->xAxis2->setScaleType(QCPAxis::stLogarithmic);
    // _ui->plot->xAxis2->setNumberPrecision(0);
    // _ui->plot->xAxis2->setNumberFormat("ebc");

    _ui->plot->xAxis2->setRange(0, 1000);
    _ui->plot->yAxis2->setRange(0, 5);

    // show seconday axis
    // _ui->plot->xAxis2->setVisible(true);
    // _ui->plot->yAxis2->setVisible(true);


    qRegisterMetaType<QVector<std::complex<int>>>("QVector<std::complex<int>>");

    connect(
        &_serialWorker, SIGNAL(receivedData(QVector<std::complex<int>>)),
        this, SLOT(serialDataReceiver(QVector<std::complex<int>>))
    );
}

MainWindow::~MainWindow()
{
    if (_serialWorker.isRunning()) {
        _serialWorker.requestInterruption();
        _serialWorker.wait();
    }

    if (_serial.isOpen()) {
        _serial.close();
    }

    delete _ui;
}

void MainWindow::serialLog(const QString &text)
{
    _ui->serialDisplay->append(text);
}

void MainWindow::serialDataReceiver(QVector<std::complex<int>> data)
{
    const double yconvert = 5.0/1024.0;
    const double xconvert = 20000.0/128.0;

    // reset data
    _xsamples.clear();
    _ysamples.clear();

    // add data to plot
    // the first frequency bucket cannot be used
    for (int i = 1; i < data.size(); i++) {
        // log data
        // serialLog(QString::number(data[i].real) + "i" + QString::number(data[i].imag));

        _ysamples.append(static_cast<double>(std::abs(data[i]) * yconvert));
        _xsamples.append(static_cast<double>((_ysamples.size() + 1) * xconvert));
    }

    // set scale axis
    _ui->plot->yAxis->setRangeLower((*std::min_element(_ysamples.begin(), _ysamples.end())) * 1.1);
    _ui->plot->yAxis->setRangeUpper((*std::max_element(_ysamples.begin(), _ysamples.end())) * 1.1);

    // plot data
    _ui->plot->graph(0)->setData(_xsamples, _ysamples, true);
    _ui->plot->replot();
}

void MainWindow::on_serialBtn_clicked()
{
    if (_serial.isOpen()) {
        // close serial thread
        if (_serialWorker.isRunning()) {
            _serialWorker.requestInterruption();
            _serialWorker.wait();
        }

        // close serial device
        _serial.close();
        serialLog("Serial device closed");

        // change text
        _ui->serialBtn->setText("Open");

        return;
    }

    // open serial device
    try {
        _serial.setPort(_ui->serialDevice->text().toStdString());
        _serial.setBaudrate(_ui->baudSpinBox->value());
        _serial.open();
        _serialWorker.start();

        serialLog("Serial device opened");

        // change text
        _ui->serialBtn->setText("Close");
    } catch (const serial::IOException &e) {
        serialLog("Failed to open serial device");
        serialLog("IOException:");
        serialLog(e.what());
    } catch (const std::exception &e) {
        serialLog("Exception:");
        serialLog(e.what());
    }
}
