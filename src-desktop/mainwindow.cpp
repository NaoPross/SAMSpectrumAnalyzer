#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    _ui->plot->addGraph();

    _ui->plot->xAxis->setLabel("t");
    _ui->plot->yAxis->setLabel("u(t)");

    _ui->plot->xAxis->setRange(0, 64);
    _ui->plot->yAxis->setRange(0, 5);

    connect(
        &_serialWorker, SIGNAL(receivedData(const QString &)),
        this, SLOT(serialDataReceiver(const QString &))
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

void MainWindow::serialDataReceiver(const QString &data)
{
    bool isnumber;
    unsigned int value = data.toInt(&isnumber);
    const double convert = 5.0/1024.0;


    if (!isnumber) {
        _xsamples.clear();
        _ysamples.clear();
        return;
    }

    // add data to plot
    _ysamples.append(static_cast<double>(value * convert));
    _xsamples.append(static_cast<double>(_ysamples.size()));

    // log data
    serialLog(QString::number(value));

    // plot data
    _ui->plot->graph(0)->setData(_xsamples, _ysamples, true);
    _ui->plot->replot();
}

void MainWindow::on_serialBtn_clicked()
{
    if (_serial.isOpen()) {
        // _serialWorker.quit();
        // while (_serialWorker.isRunning());
        if (_serialWorker.isRunning()) {
            _serialWorker.requestInterruption();
            _serialWorker.wait();
        }

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
