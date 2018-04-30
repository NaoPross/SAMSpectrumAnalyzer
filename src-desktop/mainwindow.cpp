#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaType>

#include <exception>

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
    _ui->plot->xAxis->setLabel("time");
    _ui->plot->yAxis->setLabel("voltage");
    _ui->plot->xAxis->setRange(0, 64);
    _ui->plot->yAxis->setRange(0, 5);

    // secondary axis
    _ui->plot->xAxis2->setLabel("frequency");
    _ui->plot->yAxis2->setLabel("amplitude");

    // set seconday axis to log
    // QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    // _ui->plot->xAxis2->setTicker(logTicker);
    // _ui->plot->xAxis2->setScaleType(QCPAxis::stLogarithmic);
    // _ui->plot->xAxis2->setNumberPrecision(0);
    // _ui->plot->xAxis2->setNumberFormat("ebc");

    _ui->plot->xAxis2->setRange(0, 1000);
    _ui->plot->yAxis2->setRange(0, 5);

    // show seconday axis
    _ui->plot->xAxis2->setVisible(true);
    _ui->plot->yAxis2->setVisible(true);


    qRegisterMetaType<sdt::frame>("sdt::frame");

    connect(
        &_serialWorker, SIGNAL(receivedData(const sdt::frame &)),
        this, SLOT(serialDataReceiver(const sdt::frame &))
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

void MainWindow::serialDataReceiver(const sdt::frame &data)
{
    const double convert = 5.0/1024.0;

    // reset data
    _xsamples.clear();
    _ysamples.clear();

    // add data to plot
    for (int i = 0; i < data.length; i++) {
        // log data
        serialLog(QString::number(data.samples[i].real) 
                    + " + i" + QString::number(data.samples[i].imag));

        // write only real part (for now)
        _ysamples.append(static_cast<double>(data.samples[i].real * convert));
        _xsamples.append(static_cast<double>(_ysamples.size()));
    }

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
