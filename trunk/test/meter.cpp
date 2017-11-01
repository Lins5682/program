#include "meter.h"
#include "routing.h"


#define Energy                   0
#define Voltage                  1
#define Current                  2

#define PARA_MAX_SIZE            3

#define DI_ENERGY                0x00000000
#define DI_VOLTAGE               0x00010102
#define DI_CURRENT               0x00010202

#define U8TOU32(h1,h0,l1,l0)     (((quint32)(h1)<<24)+((quint32)(h0)<<16)+((quint16)(l1)<<8)+(l0))
#define U8TOU16(h,l)             (((quint16)(h)<<8)+(l))


#define COMMON_OUT_TIME             5
MeterProtocol::MeterProtocol(Interface::Type type, uchar channel, const QString &name):
            Interface(type, channel, name)
{
    energy =  0;
    current = 0;
    voltage = 0;
}


void MeterProtocol::sendReadCmd(uchar *addr,quint8 type)
{
    QByteArray data;
    quint8 i;
    union
    {
        quint32 d;
        struct
        {
            quint8 d[4];
        }byte;
    }di;


    for(i = 0; i < 4; i++)
    {
        data.append(0xfe);
    }
    data.append(0x68);
    for(i = 0; i < 6; i++)
    {
        data.append(*addr++);
    }
    data.append(0x68);
    data.append(0x11);
    data.append(0x04);
    switch (type)
    {
        case Energy:
        {
            di.d = DI_ENERGY;
            break;
        }
        case Voltage:
        {
            di.d = DI_VOLTAGE;
            break;
        }
        case Current:
        {
            di.d = DI_CURRENT;
            break;
        }
    }
    for(i = 0; i < 4; i++)
    {
        data.append(di.byte.d[i]+0x33);
    }
    data.append(chkSum(data,3));
    data.append(0x16);
    emit sendToRouting(data);
}



int MeterProtocol::receiveHandle(const QByteArray &data)
{
    quint8 i;
    quint8 e[4];
    union
    {
        quint32 d;
        struct
        {
            quint8 d[4];
        }byte;
    }di;


    if((quint8)data.at(4) != 0x68)return -1;
    for(i = 0; i < 6; i++)
    {
        if((quint8)data.at(4 + i) != addr[i]) break;
    }
    if(i > 6)return -1;
    if((quint8)data.at(7) != 0x68)return -1;
    stateFlag = 0;
    if((quint8)data.at(8) == 0x91)
    {
        for(i = 0; i < 4; i++)
        {
            di.byte.d[i] = data.at(9+i);
        }

        switch(di.d)
        {
            case DI_ENERGY:
            {
            for(i = 0; i < 4; i++)
            {
                e[i] = (quint8)data.at(10 + i)-0x33;
            }
            e[3] &= 0x7f;

            energy = U8TOU32(e[3],e[2],e[1],e[0]);
            break;
            }
            case DI_VOLTAGE:
            {
            for(i = 0; i < 2; i++)
            {
                e[i] = (quint8)data.at(10 + i)-0x33;
            }
            voltage = U8TOU16(e[1],e[0]);
            break;
            }
            case DI_CURRENT:
            {
            for(i = 0; i < 4; i++)
            {
                e[i] = (quint8)data.at(10 + i)-0x33;
            }
            e[2] &= 0x7f;
            e[3]  = 0;

            current = U8TOU32(e[3],e[2],e[1],e[0]);
            break;
            }
        }
    }
    return 1;
}

void MeterProtocol::receiveFromRouting(const QByteArray &data)
{
    receiveHandle(data);
}
void MeterProtocol::state(void)
{
    static quint8 step = 0;
    static quint8 type = 0;
    stateCnt++;
    switch(step)
    {
        case 0:
        {
            if(stateFlag == 0)
            {
                type = (type + 1)%PARA_MAX_SIZE;
                sendReadCmd(addr,type);
                stateFlag = 1;
            }
        }
        case 1:
        {
            if(stateCnt >= COMMON_OUT_TIME)
            {
                step = 2;
            }
            break;
        }
        case 2:
        {
            if(stateCnt < COMMON_OUT_TIME)
            {
                step = 0;
            }
            else if(stateCnt >= COMMON_OUT_TIME * 2)
            {
                sendReadCmd(addr,type);
                stateCnt = COMMON_OUT_TIME;
            }
            break;
        }
    }
}

quint8 MeterProtocol::chkSum(QByteArray &data,quint8 start)
{
    quint8 i;
    quint8 chksum;

    chksum = 0;
    for(i = start; i < data.size(); i++)
    {
        chksum += (quint8)data.at(i);
    }
    return chksum;
}
quint8 MeterProtocol::BCDTonaturalNumber(quint8 b)
{
     return (((b>>4) & 0x0f) + (b & 0x0f));
}






MeterDriver ::MeterDriver(QObject *parent)
{
    currentPos  = 0;
    currentAddr = QString();
    mutex       = false;
}
void MeterDriver::sendCtrl(void)
{
     quint8        listsize;
     quint8        i;
     static quint8 paratype = 0;
     QString       str;
     bool          ok;
     quint8        addr[6];

     listsize = deviceParaList.size();
     if(mutex == true)return;
     if(listsize == 0)return;
     if(currentPos >= listsize)return;


     //初始current
     if(currentAddr == QString())
     {
         currentAddr = deviceParaList.at(0)->addr;
     }
     //addr convert
     for(i = 0; i < 12; i+=2)
     {
         str = currentAddr.mid(i,2);
         addr[i/2] = str.toInt(&ok,16);
     }
     protocol->sendReadCmd(addr,paratype);
     //加锁
     mutex = true;
     //计时开始

     if(++paratype >= MeterParaTypeSize)
     {
         //save
         paratype = 0;
         currentPos  = (currentPos + 1)%listsize;
         currentAddr = deviceParaList.at(currentPos)->addr;
     }
}


void MeterDriver::paraRefresh(const QString & addr, quint8 type, quint32 para)
{

    if(mutex == false)return;
    if(currentAddr != addr)return;


    if(deviceParaList.at(currentPos)->timeOut == true)
    {
        //清标记
        deviceParaList.at(currentPos)->timeOut = false;
    }

    switch(type)
    {
    case Energy:deviceParaList.at(currentPos)->energy = para;break;
    case Voltage:deviceParaList.at(currentPos)->voltage = para;break;
    case Current:deviceParaList.at(currentPos)->current = para;break;
    }
    //解锁
    mutex = false;
}
void MeterDriver:: MeterTimeOut(void)
{
    if(mutex == false)return;

    //标记
    deviceParaList.at(currentPos)->timeOut = true;
}
void MeterDriver::setMeterPara(MeterPara * p)
{
    deviceParaList.append(p);
}
void MeterDriver::setMeterProtocol(MeterProtocol * p)
{
    protocol = p;
    if(!protocol)
    {
        connect(protocol, SIGNAL(sendDataToDriver(const QString &,quint8,quint32)),
                    this, SLOT(paraRefresh(const QString &,quint8,quint32)));
    }
}





MeterDriver *  MeterDevice::driver=0;
MeterDevice::MeterDevice(const QString &addr)
{
    if(!driver)
    {
        driver = new MeterDriver;
    }
    para.addr  = addr;
    driver->setMeterPara(&para);
}

MeterDevice:: ~MeterDevice()
{


}











