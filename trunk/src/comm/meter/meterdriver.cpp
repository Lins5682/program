#include "meterdriver.h"
#include <QDebug>

#define METER_CH_MAX        2
#define MeterParaTypeSize   3
#define TIME_1S             1000
#define TIME_5S             5000

quint32 meterParaList[MeterParaTypeSize] =
{
    DI_ENERGY,
    DI_VOLTAGE,
    DI_CURRENT,
};

MeterDriver ::MeterDriver(QObject *parent)
{

    meterListSize = 0;

    time = new QTimer(this);
    connect(time, SIGNAL(timeout()),this,SLOT(sendCtrl()));
    time->start(TIME_1S);
}
MeterDriver::~MeterDriver()
{
    delete time;

    for(quint8 i = 0; i < meterListSize; i++)
    {
        delete meter[i]->protocol;
        delete meter[i];
    }
}

MeterDriver* MeterDriver::driver = 0;

MeterDriver* MeterDriver::meterDriver()
{
    if(!driver)
    {
        driver = new MeterDriver();
    }
    return driver;
}

void MeterDriver::sendCtrl(void)
{
     quint8   i;

     if(meterListSize == 0)return;
     for(i = 0; i < meterListSize; i++)
     {
         sendCmdCh(i);
     }
}
void MeterDriver::sendCmdCh(quint8 ch)
{
    if(ch >= METER_CH_MAX) return;
    if(meter[ch]->listsize == 0)return;
    if(meter[ch]->mutex == true)return;
    if(meter[ch]->currentMeterPos > meter[ch]->listsize)return;

    meter[ch]->protocol->sendReadCmd(meter[ch]->currentMeterAddr,
                                     meterParaList[meter[ch]->currentMeterPara]);
    //加锁
    meter[ch]->mutex = true;
    //计时开始
}

void MeterDriver:: paraRefresh(QString & name,const QByteArray & addr, quint32 type, quint32 para)
{
    quint8   i;

    if(meterListSize == 0)return;
    for(i = 0; i < meterListSize; i++)
    {
        if(name == meter[i]->protocol->name)
        {
            MetercChannel *m;
            m = meter[i];
            if(m->mutex == false)return;
            if(m->currentMeterAddr != addr)return;
            if(m->deviceParaList.at(m->currentMeterPos)->timeOut == true)
            {
                m->deviceParaList.at(m->currentMeterPos)->timeOut = false;
            }
            switch(type)
            {
            case DI_ENERGY:
                m->deviceParaList.at(m->currentMeterPos)->energy = para;break;
            case DI_VOLTAGE:
                m->deviceParaList.at(m->currentMeterPos)->voltage = para;break;
            case DI_CURRENT:
                m->deviceParaList.at(m->currentMeterPos)->current = para;break;
            }
            if(++m->currentMeterPara >= MeterParaTypeSize)
            {
                m->currentMeterPara = 0;
                m->currentMeterPos  = (m->currentMeterPos + 1) % m->listsize;
                m->currentMeterAddr = m->deviceParaList[m->currentMeterPos]->addr;
            }
            m->mutex = false;
        }
    }
}
void MeterDriver:: MeterTimeOut(void)
{
    //if(mutex == false)return;

    //标记
    //deviceParaList.at(currentMeterPos)->timeOut = true;
}

int MeterDriver::setMeterPara(MeterPara *p,quint32 ch)
{
    if(!p)return -1;
    if(p->ch >= METER_CH_MAX)return -1;

    meter[ch]->deviceParaList.append(p);
    meter[ch]->listsize = meter[ch]->deviceParaList.size();
    if(meter[ch]->listsize == 1)
    {
        meter[ch]->currentMeterAddr = meter[ch]->deviceParaList.at(0)->addr;
    }
    return 1;
}
int MeterDriver::removeMeterPara(MeterPara *p)
{
    if(!p)return -1;
    if(p->ch >= METER_CH_MAX)return -1;

    quint8 size = meter[p->ch]->listsize;
    if(size == 0 )return -1;

    MetercChannel *m;
    m = meter[p->ch];
    for(quint8 i = 0; i < size; i++)
    {
       if(m->deviceParaList.at(i) == p)
       {
           m->deviceParaList.removeAt(i);
           m->listsize = m->deviceParaList.size();
           if(m->listsize > 0)
           {
               if(m->currentMeterAddr == p->addr)
               {
                   m->currentMeterAddr = m->deviceParaList[0]->addr;
                   m->currentMeterPara = 0;
                   m->currentMeterPos  = 0;
                   m->mutex            = false;
               }
           }
           else
           {
               m->currentMeterAddr = 0;
               m->currentMeterPara = 0;
               m->currentMeterPos  = 0;
               m->listsize         = 0;
               m->mutex            = false;
           }
           return 1;
       }
    }
    return -1;
}

int MeterDriver::setMeterChannel(MeterProtocol *p)
{
    if(!p)return -1;

    if(meterListSize >= METER_CH_MAX)return -1;

    meter.append(new struct MetercChannel);
    meterListSize = meter.size();
    quint8 i = meterListSize - 1;
    meter[i]->protocol = p;
    meter[i]->currentMeterAddr = 0;
    meter[i]->currentMeterPara = 0;
    meter[i]->currentMeterPos  = 0;
    meter[i]->listsize         = 0;
    meter[i]->mutex            = false;
    connect(meter[i]->protocol,SIGNAL(sendDataToDriver(QString &,QByteArray,quint32,quint32)),
            this,SLOT(paraRefresh(QString &,QByteArray,quint32,quint32)));
    return 1;
}

void MeterDriver::deviceNext(void)
{
    QTimer *time = new QTimer(this);

    connect( time, SIGNAL(timeout()), this, SLOT(sendCtrl()));
    time->start(TIME_1S);

}

