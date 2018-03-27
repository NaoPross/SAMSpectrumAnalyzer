#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
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
    void receivedData(QString &data);

private:
    bool running;
    serial::Serial &_serial;
};

#endif // SERIALWORKER_H
