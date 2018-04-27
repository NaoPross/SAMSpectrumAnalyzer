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
        QString data;

        try {
            // TODO: implement a protocol
            while ((_serial.available() <= 8)&& !isInterruptionRequested());
            // _serial.waitReadable();

            data = QString::fromStdString(_serial.readline(65536, "\n\r"));
        } catch (serial::IOException e) {
            // TODO: handle error on the GUI
            emit receivedData("IOException");
            break;
        }

        emit receivedData(data);

        // _serial.flushOutput();
    }
}
