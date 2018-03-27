#include "serialworker.h"

SerialWorker::SerialWorker(serial::Serial &serial) :
    _serial(serial)
{

}

SerialWorker::~SerialWorker()
{

}

void SerialWorker::run() override
{
    _running = true;
    while (_running) {
        while (!_serial.available() && _running);

        QString data = QString::fromStdString(_serial.readline());
        emit receivedData(data);

        _serial.flushOutput();
    }
}
