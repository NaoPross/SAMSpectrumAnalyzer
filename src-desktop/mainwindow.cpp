#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaType>

#include <exception>
#include <algorithm>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _serial(),
    _serialWorker(_serial)
{
    // qt setup ui
    _ui->setupUi(this);
    
    // set default serial device
    _ui->serialDevice->setText(
#ifdef _WIN32
        "COM1"
#elif defined(__linux__)
        "/dev/ttyUSB0"
#endif
    );

    // enable automatic axis scaling
    _ui->adjustAxisCheckBox->setChecked(true);
    _ui->setAxisBtn->setEnabled(false);

    // graph 0 on bottom and left axis
    _ui->plot->addGraph(_ui->plot->xAxis, _ui->plot->yAxis);

    // set graph data
    _xsamples.clear();
    _ysamples.clear();

    // graph 1 on top and right axis
    // _ui->plot->addGraph(_ui->plot->xAxis2, _ui->plot->yAxis2);

    // primary axis
    _ui->plot->xAxis->setLabel("Frequency");
    _ui->plot->yAxis->setLabel("Amplitude");

    _ui->plot->xAxis->setRange(0, 10000);
    _ui->plot->yAxis->setRange(0, 5);

    // secondary axis
    // _ui->plot->xAxis2->setLabel("");
    // _ui->plot->yAxis2->setLabel("");

    // set seconday axis to log
    // QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    // _ui->plot->xAxis2->setTicker(logTicker);
    // _ui->plot->xAxis2->setScaleType(QCPAxis::stLogarithmic);
    // _ui->plot->xAxis2->setNumberPrecision(0);
    // _ui->plot->xAxis2->setNumberFormat("ebc");

    // _ui->plot->xAxis2->setRange(0, 1000);
    // _ui->plot->yAxis2->setRange(0, 5);

    // show seconday axis
    // _ui->plot->xAxis2->setVisible(true);
    // _ui->plot->yAxis2->setVisible(true);


    // register metatype to use on qt events (signals)
    qRegisterMetaType<QVector<std::complex<int>>>("QVector<std::complex<int>>");

    // serial device received data callback
    connect(
        &_serialWorker, SIGNAL(receivedData(QVector<std::complex<int>>)),
        this, SLOT(serialDataReceiver(QVector<std::complex<int>>))
    );

    // combobox changed index callback
    connect(_ui->plotTypeSelCombo, 
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){on_plotTypeSelCombo_currentIndexChanged(index);}
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
    // constants to convert from integer data to double
    //
    const double xConvert = 20000.0/128.0;
    double yConvert;
    // 
    const double yAdjustLowerFactor = 1.0;
    const double yAdjustUpperFactor = 1.1;

    // reset data
    _xsamples.clear();
    _ysamples.clear();

    switch (_ui->plotTypeSelCombo->currentIndex()) {
    case 0: // amplitude
    case 2: // complex
    case 3: // real 
    case 4: // imaginary
        _ui->plot->yAxis->setRange(0, 5);
        yConvert = 5.0/1024.0;
        break;

    case 1: // phase
        _ui->plot->yAxis->setRange(-M_PI, M_PI);
        yConvert = 2.0 * M_PI;
        break;
    }



    // add data to plot
    // the first frequency bucket and the second half cannot be used
    for (int i = 1; i < data.size() / 2; i++) {
        // log data, disabled for better performance
        // serialLog(QString::number(data[i].real()) + "i" + QString::number(data[i].imag()));
        
        double yvalue = 0;
        switch (_ui->plotTypeSelCombo->currentIndex()) {
        case 0:
            // amplitude
            yvalue = std::abs(data[i]);
            break;
        case 1:
            // phase
            yvalue = std::arg(data[i]);
            break;
        case 2:
            // complex
            // TODO
            break;
        case 3:
            // real
            yvalue = data[i].real();
            break;
        case 4:
            // imaginary
            yvalue = data[i].imag();
            break;
        }

        _ysamples.append(static_cast<double>(yvalue * yConvert));
        _xsamples.append(static_cast<double>((_ysamples.size() + 1) * xConvert));
    }

    // set scale axis
    if (_ui->adjustAxisCheckBox->isChecked()) {
        // only y axis needs adjustments
        _ui->plot->yAxis->setRangeLower((
                *std::min_element(_ysamples.begin(), _ysamples.end())
         ) * yAdjustLowerFactor);

        _ui->plot->yAxis->setRangeUpper((
                *std::max_element(_ysamples.begin(), _ysamples.end())
        ) * yAdjustUpperFactor);
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

        // change text and enable widgets
        _ui->serialBtn->setText("Open");
        _ui->serialDevice->setEnabled(true);
        _ui->baudSpinBox->setEnabled(true);

        return;
    }

    // open serial device
    try {
        _serial.setPort(_ui->serialDevice->text().toStdString());
        _serial.setBaudrate(_ui->baudSpinBox->value());
        _serial.open();
        _serialWorker.start();

        serialLog("Serial device opened");

        // change text and disable widgets
        _ui->serialBtn->setText("Close");
        _ui->serialDevice->setEnabled(false);
        _ui->baudSpinBox->setEnabled(false);
    } catch (const serial::IOException &e) {
        serialLog("Failed to open serial device");
        serialLog("IOException:");
        serialLog(e.what());
    } catch (const std::exception &e) {
        serialLog("Exception:");
        serialLog(e.what());
    }
}

void MainWindow::on_adjustAxisCheckBox_toggled(bool value)
{
    _ui->setAxisBtn->setEnabled(!value);
}

void MainWindow::on_plotTypeSelCombo_currentIndexChanged(int index)
{
    switch (index) {
    case 0: // amplitude
    case 2: // complex
    case 3: // real 
    case 4: // imaginary
        {
            QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
            _ui->plot->yAxis->setTicker(fixedTicker);
        }
        break;

    case 1: // phase
        {
            QSharedPointer<QCPAxisTickerPi> piTicker(new QCPAxisTickerPi);
            _ui->plot->yAxis->setTicker(piTicker);
        }
        break;
    }
}
