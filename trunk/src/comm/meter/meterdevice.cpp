#include "meterdevice.h"

MeterDriver *  MeterDevice::driver=0;
quint8 MeterDevice::num = 0;
MeterDevice::MeterDevice(const QByteArray &addr,quint8 ch, quint8 type)
{
    if(!driver)
    {
        driver = MeterDriver::meterDriver();
    }
    para.addr  = addr;
    para.ch = ch;
    para.current = 0;
    para.energy  = 0;
    para.voltage = 0;
    para.timeOut = false;
    driver->setMeterPara(&para,ch);
    MeterDevice::num ++;
}

MeterDevice:: ~MeterDevice()
{
    driver->removeMeterPara(&para);
    MeterDevice::num--;
}
