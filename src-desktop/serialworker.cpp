#include "serialworker.h"

#include <QMutexLocker>
#include <QMutex>

SerialWorker::SerialWorker(serial::Serial &serial) :
    _serial(serial), _mutex()
{

}

SerialWorker::~SerialWorker()
{

}

void SerialWorker::run()
{
    while (true) {
        while (!_serial.available()) {
            // locks the mutex until it goes out of scope
            QMutexLocker locker(&_mutex);
            if (!_running)
                break;
        }

        _mutex.lock();
        if (!_running)
            break;
        _mutex.unlock();

        QString data = QString::fromStdString(_serial.readline());
        emit receivedData(data);
        _serial.flushOutput();
    }
}

void SerialWorker::stop()
{
    QMutexLocker locker(&_mutex);
    _running = false;
}
