#include "serialworker.h"

#include <QMutexLocker>

#include <string>
#include <complex>


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
        QVector<std::complex<int>> data;

        while (!isInterruptionRequested()) {
            // wait for serial buffer to accumulate at least 12 bytes
            while (_serial.available() < 14)
                if (isInterruptionRequested())
                    return;

            // read data
            QString str = QString::fromStdString(_serial.readline());
                
            // start of data
            if (str.trimmed() == "S") {
                data.clear();
            }
            // end of data
            else if (str.trimmed() == "E") {
                break;
            }
            // data
            else {
                QStringList valueStr = str.trimmed().split('i');
                if (valueStr.size() < 2)
                    continue;

                bool isInt[2];
                std::complex<int> value(
                    valueStr.at(0).toInt(&isInt[0]),
                    valueStr.at(1).toInt(&isInt[1])
                );

                // if (!isInt[0] || !isInt[1])
                //     continue

                data.push_back(value);
            }
        }

        emit receivedData(data);
    }
}
