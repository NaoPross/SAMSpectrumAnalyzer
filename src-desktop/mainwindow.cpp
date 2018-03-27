#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <exception>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)

{
    _ui->setupUi(this);

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(readSerialLog()));

    _ui->serialDevice->setText(
#ifdef WIN32
        "COM1"
#elif defined(__linux__)
        "/dev/ttyUSB0"
#endif
    );
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _timer;

    if (_serialDevice != nullptr)
        delete _serialDevice;
}

void MainWindow::serialLog(const QString &text)
{
    _ui->serialDisplay->append(text);
}

void MainWindow::readSerialLog()
{
    if (_serialDevice == nullptr)
        return;

    if (!_serialDevice->isOpen())
        return;

    // for (const std::string &line : _serialDevice->readlines()) {
    //     _ui->serialDisplay->append(QString::fromStdString(line));
    // }

    _ui->serialDisplay->append(QString::fromStdString(_serialDevice->readline()));
    _serialDevice->flushOutput();

}

void MainWindow::on_serialBtn_clicked()
{
    if (_serialDevice != nullptr) {

        if (_serialDevice->isOpen()) {
            _timer->stop();
            _serialDevice->close();
        }

        serialLog("Serial device closed");
        delete _serialDevice;
        _serialDevice = nullptr;

        // change text
        _ui->serialBtn->setText("Open");

        return;
    }


    // open serial device
    try {
        _serialDevice = new serial::Serial(
            _ui->serialDevice->text().toStdString(),
            _ui->baudSpinBox->value()
        );

        serialLog("Serial device opened");
    } catch (const serial::IOException &e) {
        serialLog("Exception:");
        serialLog(e.what());
        serialLog("Failed to open serial device");
    } catch (const std::exception &e) {
        serialLog(e.what());
    }

    // start timer task
    _timer->start(100);

    // change text
    if (_serialDevice != nullptr) {
        _ui->serialBtn->setText("Close");
    }
}
