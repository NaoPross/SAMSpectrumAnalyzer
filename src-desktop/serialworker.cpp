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
        while ((_serial.available() <= 8)&& !isInterruptionRequested());

        // for (std::string line : _serial.readlines(65536, "\n\r")) {
        //     QString data = QString::fromStdString(line);
        //     emit receivedData(data);
        // }

        _serial.waitReadable();
        QString data = QString::fromStdString(_serial.readline(65536, "\n\r"));
        emit receivedData(data);
        // _serial.flushOutput();
    }
}
