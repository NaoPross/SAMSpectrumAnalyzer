#include "serialworker.h"

#include <QMutexLocker>

#include <string>
#include <iostream> // debug


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
        QVector<sam::complex_uint16_t> data;
        data.clear();

        while (!isInterruptionRequested()) {
            // wait for serial buffer to accumulate at least 12 bytes
            while (_serial.available() < 12) {
                if (isInterruptionRequested())
                    return;
            }

            // read data
            QString str = QString::fromStdString(_serial.readline());
                
            // start of data
            if (str.trimmed() == "S") {
                continue;
            }
            // end of data
            else if (str.trimmed() == "E") {
                break;
            }
            // data
            else {
                bool isUInt;
                QStringList valueStr = str.trimmed().split('i');
                sam::complex_uint16_t value;

                if (valueStr.size() < 2)
                    continue;

                value.real = valueStr.at(0).toUInt(&isUInt);
                if (!isUInt)
                    continue;

                value.imag = valueStr.at(1).toUInt(&isUInt);
                if (!isUInt)
                    continue;

                data.push_back(value);
            }
        }

        emit receivedData(data);
    }
}
