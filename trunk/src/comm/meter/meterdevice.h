#ifndef METERDEVICE_H
#define METERDEVICE_H
#include "meterdriver.h"




class MeterDevice
{
public:
    explicit MeterDevice(const QByteArray &addr,quint8 ch, quint8 type);
    ~MeterDevice();

    QString   getAddr(void){return para.addr;}
    quint32   getEnergy(void){return para.energy;}
    quint32   getVoltage(void){return para.voltage;}
    quint32   getCurrent(void){return para.current;}
    static quint8 getmeterDeviceNum(void){return MeterDevice::num;}
private :
    static quint8 num;
    static MeterDriver *driver;
    MeterPara para;
};

#endif // METERDEVICE_H
