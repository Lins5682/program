#ifndef INTERFACESERIAL_H
#define INTERFACESERIAL_H

#include "interface.h"

class Posix_QextSerialPort;

class InterfaceSerial : public Interface
{
    Q_OBJECT
public:
    explicit InterfaceSerial(const QString &name, \
        const QString &deviceName, \
        SerialBaudRate baudRate = SERIAL_BAUD_57600, \
        SerialParity parity = SERIAL_PARITY_NONE);

    virtual bool initialize();

    void setBaudRate(Interface::SerialBaudRate baudRate);
    void setParity(Interface::SerialParity parity);

public Q_SLOTS:
    virtual void receiveFromRouting(QByteArray data);

private:
    // 设备
    Posix_QextSerialPort *device;
    QString deviceName;
    SerialBaudRate baudRate;
    SerialParity parity;

private slots:
    void readDevice(void);
};

#endif // INTERFACESERIAL_H
