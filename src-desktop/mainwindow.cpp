#include "mainwindow.h"
#include "exportimagedialog.h"

#include "ui_mainwindow.h"

#include <QMetaType>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

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
    // _ui->setAxisBtn->setEnabled(false);

    // graph 0 on bottom and left axis
    _ui->plot->addGraph(_ui->plot->xAxis, _ui->plot->yAxis);
    // graph 1 on bottom and left axis
    // _ui->plot->addGraph(_ui->plot->xAxis, _ui->plot->yAxis);

    // graph 0 line style
    // _ui->plot->graph(0)->setLineStyle(QCPGraph::lsImpulse);
    // _ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));

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

    _ui->plot->xAxis->setNumberFormat("f");
    _ui->plot->yAxis->setNumberFormat("gb");

    _ui->plot->xAxis->setNumberPrecision(0);
    _ui->plot->yAxis->setNumberPrecision(0);

    _defaultTicker = _ui->plot->xAxis->ticker();
    _logTicker = QSharedPointer<QCPAxisTickerLog>(new QCPAxisTickerLog);

    // secondary axis
    // _ui->plot->xAxis2->setLabel("");
    // _ui->plot->yAxis2->setLabel("");

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

    // connect serial exception to close event
    connect(
        &_serialWorker, SIGNAL(exception()),
        this, SLOT(closeSerialDevice())
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

/* private slots */

void MainWindow::serialDataReceiver(QVector<std::complex<int>> data)
{
    // constants to convert from integer data to double
    const double yAdjustLowerFactor = 1.0;
    const double yAdjustUpperFactor = 1.1;

    const double xConvert = 20000.0/128.0;
    double yConvert;

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
            // TODO use graph(1)
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
        double min = *std::min_element(_ysamples.begin(), _ysamples.end());
        double max = *std::max_element(_ysamples.begin(), _ysamples.end());

        // min = (min < .1) ? .1 : min;

        // only y axis needs adjustments
        _ui->plot->yAxis->setRangeLower(min * yAdjustLowerFactor);
        _ui->plot->yAxis->setRangeUpper(max * yAdjustUpperFactor);
    }

    // plot data
    _ui->plot->graph(0)->setData(_xsamples, _ysamples, true);
    _ui->plot->replot();
}

bool MainWindow::openSerialDevice()
{
    if (_serial.isOpen())
        return true;

    try {
        _serial.setPort(_ui->serialDevice->text().toStdString());
        _serial.setBaudrate(_ui->baudSpinBox->value());
        _serial.open();
        _serialWorker.start();

        serialLog("Serial device opened");
        return true;

    } catch (const serial::IOException &e) {
        serialLog("Failed to open serial device");
        serialLog("IOException:");
        serialLog(e.what());
    } catch (const std::exception &e) {
        serialLog("Exception:");
        serialLog(e.what());
    }

    return false;
}

void MainWindow::closeSerialDevice()
{
    if (!_serial.isOpen())
        return;

    if (_serialWorker.isRunning()) {
        _serialWorker.requestInterruption();
        _serialWorker.wait();
    }

    // close serial device
    _serial.close();
    serialLog("Serial device closed");
}

/* private slots for ui events */

void MainWindow::on_serialBtn_clicked()
{
    if (_serial.isOpen()) {
        closeSerialDevice();

        // change text and enable widgets
        _ui->serialBtn->setText("Open");
        _ui->serialDevice->setEnabled(true);
        _ui->baudSpinBox->setEnabled(true);

    } else if (openSerialDevice()) {
        // change text and disable widgets
        _ui->serialBtn->setText("Close");
        _ui->serialDevice->setEnabled(false);
        _ui->baudSpinBox->setEnabled(false);
    }
}

void MainWindow::on_adjustAxisCheckBox_toggled(bool checked)
{
    _ui->plot->replot();
}

void MainWindow::on_logFreqAxisCheckBox_toggled(bool checked)
{
    if (checked) {
        // set log axis
        _ui->plot->xAxis->setTicker(_logTicker);
        _ui->plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        _ui->plot->xAxis->setNumberFormat("ebc");
        _ui->plot->xAxis->setRangeLower(100);
    } else {
        _ui->plot->xAxis->setTicker(_defaultTicker);
        _ui->plot->xAxis->setScaleType(QCPAxis::stLinear);
        _ui->plot->xAxis->setNumberFormat("f");
        _ui->plot->xAxis->setRangeLower(0);
    }

    _ui->plot->replot();
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

    _ui->plot->replot();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionSave_data_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                "Save Data", QDir::homePath(),
                "Comma Separated Values (*.csv)");

    if (filename.isNull())
        return;

    // check if there is any data to write
    if (_xsamples.empty() || _ysamples.empty()) {
        serialLog("No data to write");
        return;
    }

    // check that file has an extensions
    QFileInfo fileInfo(filename);
    if (fileInfo.suffix() == "")
        filename += ".csv";

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        serialLog("Failed to open file:");
        serialLog(filename);
        return;
    }

#ifdef _WIN32
    const QString eol = "\n\r";
#else
    const QString eol = "\n";
#endif

    QTextStream out(&file);

    // header
    out << "frequency, magnitude" << eol;

    const int samples_size = (_xsamples.size() > _ysamples.size())
            ? _xsamples.size() : _ysamples.size();

    for (int i = 0; i < samples_size; i++) {
        out << QString::number(_xsamples[i]) << ", "
            << QString::number(_ysamples[i]) << eol;
    }

    file.close();
}

void MainWindow::on_actionExport_image_triggered()
{
    // ask output image size
    ExportImageDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this,
        "Save plot image", QDir::homePath(),
        "Vector Image (*.pdf);;Bitmap Image (*.bmp);;Compressed Image (*.png *.jpg)",
        &selectedFilter
    );

    // the user did not select a file
    if (filename.isNull())
        return;

    QFileInfo fileInfo(filename);

    // if the user has not specified an extension
    if (fileInfo.suffix() == "") {
        if (selectedFilter == "Vector Image (*.pdf)")
            filename += ".pdf";
        else if (selectedFilter == "Bitmap Image (*.bmp)")
            filename += ".bmp";
        else if (selectedFilter == "Compressed Image (*.png *.jpg)")
            filename += ".png";

        // update FileInfo
        fileInfo = QFileInfo(filename);
    }

    // write data
    if (fileInfo.suffix() == "pdf")
        _ui->plot->savePdf(filename,
                           dialog.getImageWidth(),
                           dialog.getImageHeight());
    else if (fileInfo.suffix() == "bmp")
        _ui->plot->saveBmp(filename,
                           dialog.getImageWidth(),
                           dialog.getImageHeight());
    else if (fileInfo.suffix() == "png")
        _ui->plot->savePng(filename,
                           dialog.getImageWidth(),
                           dialog.getImageHeight());
    else if (fileInfo.suffix() == "jpg" || fileInfo.suffix() == "jpeg")
        _ui->plot->saveJpg(filename,
                           dialog.getImageWidth(),
                           dialog.getImageHeight());
    else {
        serialLog("Invalid file type: ");
        serialLog(filename);
    }
}
