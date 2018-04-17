#include "serialworker.h"

#include <QMutexLocker>

SerialWorker::SerialWorker(serial::Serial &serial) :
    _mutex(), _serial(serial)
{

}

SerialWorker::~SerialWorker()
{

}

void SerialWorker::run()
{
    while (isRunning()) {
        QMutexLocker locker(&_mutex);
        while (!_serial.available() && isRunning());

        QString data = QString::fromStdString(_serial.readline());
        emit receivedData(data);
        _serial.flushOutput();
    }
}
