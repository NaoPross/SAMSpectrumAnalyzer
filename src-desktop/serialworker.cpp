#include "serialworker.h"

SerialWorker::SerialWorker(serial::Serial &serial) :
    _serial(serial)
{

}

SerialWorker::~SerialWorker()
{

}

void SerialWorker::run()
{
    while (_running) {
        while (!_serial.available() && _running);

        QString data = QString::fromStdString(_serial.readline());
        emit receivedData(data);
        _serial.flushOutput();

    }
}

void SerialWorker::stop()
{
    _running = false;
}
