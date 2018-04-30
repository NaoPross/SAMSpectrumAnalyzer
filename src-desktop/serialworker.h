#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QVector>

#include "serial/serial.h"
#include "../src-common/complex.hpp"

class SerialWorker : public QThread
{
    Q_OBJECT
public:
    SerialWorker() = delete;
    explicit SerialWorker(serial::Serial &serial);
    ~SerialWorker();

    void run() override;

signals:
    void receivedData(QVector<sam::complex_uint16_t> data);

private:
    QMutex _mutex;
    serial::Serial &_serial;
};

#endif // SERIALWORKER_H
