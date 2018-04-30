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
        
        uint8_t header[2];
        sdt::frame data;

        // TODO: add try / catch with ui altert
        while (!_serial.available()) {
            if (isInterruptionRequested())
                return;
        }

        // wait for header
        do {
            _serial.read(header, 2);

            if (isInterruptionRequested())
                return;

        } while ((header[0]<<8 | header[1]) != sdt::SDT_HEADER);

        // 
        data.header = sdt::SDT_HEADER;
        data.length = *reinterpret_cast<const uint16_t *>(_serial.read(sizeof(uint16_t)).data());

        // allocate data.length
        if (!data.realloc())
            continue;

        // read the samples
        std::memcpy(data.samples,
            reinterpret_cast<const sdt::complex_uint16_t *>(
                _serial.read(data.length * sizeof(sdt::complex_uint16_t)).data()),
            data.length
        );


        emit receivedData(data);
    }
}
