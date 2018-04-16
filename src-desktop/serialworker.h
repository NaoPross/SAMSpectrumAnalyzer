#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "serial/serial.h"

class SerialWorker : public QThread
{
    Q_OBJECT
public:
    SerialWorker() = delete;
    explicit SerialWorker(serial::Serial &serial);
    ~SerialWorker();

    void run() override;
    void stop();

signals:
    void receivedData(QString &data);

private:
    volatile bool _running;
    serial::Serial &_serial;
    QMutex _mutex;
};

#endif // SERIALWORKER_H
