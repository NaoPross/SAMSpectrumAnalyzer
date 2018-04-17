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

signals:
    void receivedData(const QString &data);

private:
    QMutex _mutex;
    serial::Serial &_serial;
};

#endif // SERIALWORKER_H
