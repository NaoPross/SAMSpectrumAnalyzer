#include "serialworker.h"

#include <QMutexLocker>
#include <string>

SerialWorker::SerialWorker(serial::Serial &serial) :
    _mutex(), _serial(serial)
{

}

SerialWorker::~SerialWorker()
{

}

void SerialWorker::run()
{
    while (!isInterruptionRequested()) {
        QMutexLocker locker(&_mutex);
        while (!_serial.available() && !isInterruptionRequested());

        for (std::string line : _serial.readlines()) {
            QString data = QString::fromStdString(line);
            emit receivedData(data);
        }

        // QString data = QString::fromStdString(_serial.readline());
        // emit receivedData(data);
        _serial.flushOutput();
    }
}
