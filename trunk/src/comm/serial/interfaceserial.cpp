#include "interfaceserial.h"
#include "posix_qextserialport.h"
#include <QDebug>

InterfaceSerial::InterfaceSerial(const QString &name,\
    const QString &deviceName, SerialBaudRate baudRate, SerialParity parity) :
    Interface(CI_SERIAL, name)
{
    this ->deviceName = deviceName;
    this ->baudRate = baudRate;
    this ->parity = parity;
}

bool InterfaceSerial::initialize()
{
    if(!Interface::initialize()) return false;
    error ->messageType(Error::INITIALIZE_START);
    // 设备
    device = new Posix_QextSerialPort(deviceName, QextSerialBase::Polling);
    device ->open(QIODevice::ReadWrite);
    if(!device -> isOpen())
    {
        faultFlag = true;
        error ->handle(-3);
        error ->message(QString("open serial port failed [x]"));
        return false;
    }
    // 定时器
    readTimer ->setSingleShot(false);
    readTimer ->setInterval(10);
    QObject::connect(readTimer, SIGNAL(timeout()), this, SLOT(readDevice()));
    readTimer ->start();
    // 标志位
    initializedFlag = true;
    availableFlag = true;
    // 硬件设置
    setBaudRate(baudRate);
    device ->setDataBits(DATA_8);
    setParity(parity);
    device ->setStopBits(STOP_1);
    device ->setFlowControl(FLOW_OFF);
    device ->setTimeout(5);
    // 消息
    error ->messageType(Error::INITIALIZE_OK);
    return true;
}

void InterfaceSerial::setBaudRate(Interface::SerialBaudRate baudRate)
{
    error ->handleBase(ERROR_SET);
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    BaudRateType deviceBaudRate;
    switch(baudRate)
    {
        case Interface::SERIAL_BAUD_1200: deviceBaudRate = BAUD1200; break;
        case Interface::SERIAL_BAUD_2400: deviceBaudRate = BAUD2400; break;
        case Interface::SERIAL_BAUD_4800: deviceBaudRate = BAUD4800; break;
        case Interface::SERIAL_BAUD_9600: deviceBaudRate = BAUD9600; break;
        case Interface::SERIAL_BAUD_19200: deviceBaudRate = BAUD19200; break;
        case Interface::SERIAL_BAUD_38400: deviceBaudRate = BAUD38400; break;
        case Interface::SERIAL_BAUD_57600: deviceBaudRate = BAUD57600; break;
        case Interface::SERIAL_BAUD_76800: deviceBaudRate = BAUD76800; break;
        case Interface::SERIAL_BAUD_115200: deviceBaudRate = BAUD115200; break;
        default: deviceBaudRate = BAUD9600;
    }
    device ->setBaudRate(deviceBaudRate);
}

void InterfaceSerial::setParity(Interface::SerialParity parity)
{
    error ->handleBase(ERROR_SET);
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    ParityType deviceParity;
    switch(parity)
    {
        case Interface::SERIAL_PARITY_NONE: deviceParity = PAR_NONE; break;
        case Interface::SERIAL_PARITY_ODD: deviceParity = PAR_ODD; break;
        case Interface::SERIAL_PARITY_EVEN: deviceParity = PAR_EVEN; break;
        default: deviceParity = PAR_NONE;
    }
    device ->setParity(deviceParity);
}

void InterfaceSerial::receiveFromRouting(QByteArray data)
{
    error ->handleBase(ERROR_RECEIVE);
    // 不可用
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    // 检查数据格式 STREAM
    if(!checkFrame(data, STREAM))
    {
        error ->handle(-2);
        return;
    }
    removeFrame(data);
    device ->write(data);
}

void InterfaceSerial::readDevice(void)
{
    error ->handleBase(ERROR_READ);
    // 不可用
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    QByteArray data;
    data = device ->readAll();
    if(data.size() > 0)
    {
        // disable appendInt(data, STREAM);
        emit sendToRouting(data);
    }
}
