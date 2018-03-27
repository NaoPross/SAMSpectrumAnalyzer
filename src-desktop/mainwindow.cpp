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

    connect(
        &_serialWorker, SIGNAL(&SerialWorker::receivedData(QString &data)),
        this, SLOT(&serialLog(const QString &data))
    );
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::serialLog(const QString &text)
{
    _ui->serialDisplay->append(text);
}

void MainWindow::on_serialBtn_clicked()
{
    if (_serial.isOpen()) {
        _serialWorker.stop();
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
    } catch (const serial::IOException &e) {
        serialLog("Failed to open serial device");
        serialLog("IOException:");
        serialLog(e.what());
    } catch (const std::exception &e) {
        serialLog("Exception:");
        serialLog(e.what());
    }

    // change text
    _ui->serialBtn->setText("Close");
}
