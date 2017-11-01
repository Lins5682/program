#include "management.h"
#include <QStringList>

QList<uchar> State::fromString(QString str, uchar length, uchar fillchar)
{
    if(str.size() < length)
    {
        QString strTmp;
        strTmp.fill(fillchar, length - str.size());
        str.insert(0, strTmp);
    }

    QList<uchar> data;
    for(int i = 0; i < length; i++)
    {
        data.append(str.at(i).toAscii());
    }
    return data;
}

template <typename T>
QList<uchar> State::fromInt(const T udata)
{
    QList<uchar> dataList;
    u_int8_t ulen = sizeof(udata);
    if(ulen > 3)
    {
        dataList.append((udata >> 24) & 0x000000ff);
    }
    if(ulen > 2)
    {
        dataList.append((udata >> 16) & 0x000000ff);
    }
    if(ulen > 1)
    {
        dataList.append((udata >> 8) & 0x000000ff);
    }
    dataList.append(udata & 0x000000ff);
    return dataList;
}

QByteArray State::fromList(QList<uchar> dlist)
{
    QByteArray darray;
    for(int i=0; i<dlist.size(); i++)
    {
        darray.append(dlist.at(i));
    }
    return darray;
}

u_int32_t State::getTimeStamp()
{
    u_int32_t stamp;
    QDateTime dtStamp;
    dtStamp = QDateTime::fromString("1970 01-01 00:00:00", "yyyy MM-dd hh:mm:ss");
    stamp = QDateTime::currentDateTime().secsTo(dtStamp);
    qDebug("second: %d", stamp);
    return stamp;
}

void State::handle()
{
    udata.clear();
    dataSegment();
    emit write(fromList(udata));

    /*接收数据帧*/
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->start(2000);

    QEventLoop loop;
    while(timer->isActive())
    {
        QTimer::singleShot(1, &loop, SLOT(quit()));
        loop.exec();

//        if(ISReadEnd == true) timer->stop();
    }

    delete timer;
}

Login::~Login()
{
    delete management;
}

void Login::dataSegment()
{
    /*设备类型*/
    udata.append(management->devicetype);
    /*额定功率*/
    udata.append(fromInt(management->ratedpower));
    /*额定电压*/
    udata.append(fromInt(management->ratedvoltage));
    /*经纬度*/
    QString coordinate = management->longitude;
    udata.append(fromString(coordinate.remove("."), 12));
    QString latitude = management->latitude;
    udata.append(fromString(latitude.remove("."), 12));
    /*所属电站编号*/
    udata.append(fromString(management->station, 13));
    /*所属地区编号*/
    udata.append(fromString(management->arear, 13));
    /*营运类型*/
    udata.append(fromInt(management->servertype));
    /*桩内编号*/
    udata.append(fromInt(management->insidenumber));
    /*枪口个数*/
    udata.append(management->gun.size());
    /*充电枪信息*/
    for(int i = 0; i < management->gun.size(); i++)
    {
        /*停车位号*/
        udata.append(fromInt(management->gun.at(i).parkspace));
        /*充电枪接口类型*/
        udata.append(management->gun.at(i).type);
    }

//    qDebug() << "Login..." << QTime::currentTime().toString();
}

StartCharge::~StartCharge()
{
    delete management;
}

void StartCharge::dataSegment()
{
    /*启动枪口号*/
    udata.append(gunnumber);
    /*启动方式*/
    udata.append(mode);
    /*账号*/
    udata.append(fromString(accountNO, 30));
    /*余额*/
    udata.append(fromInt(balance));
    /*预付费金额*/
    udata.append(fromInt(prepaidamount));
    /*免费金额*/
    udata.append(fromInt(freeamount));
    /*当前电表读数*/
    udata.append(fromInt(currentmeter));
}

StopCharge::~StopCharge()
{
    delete management;
}

void StopCharge::dataSegment()
{
    /*停止枪口号*/
    udata.append(gunnumber);
    /*停止方式*/
    udata.append(reason);
    /*账号*/
    udata.append(fromString(accountNO, 30));
    /*余额*/
    udata.append(fromInt(balance));
    /*预付费金额*/
    udata.append(fromInt(prepaidamount));
    /*免费金额*/
    udata.append(fromInt(freeamount));
    /*当前电表读数*/
    udata.append(fromInt(currentmeter));
    /*已充电量*/
    udata.append(fromInt(chargequantity));
}


Fault::~Fault()
{
    delete management;
    delete faultTable;
}

void Fault::dataSegment(QList<uchar> type)
{
    uchar index, data;
    QList<uchar> tableList;
    for(int i = 0; i < type.size(); i++)
    {
        if(faultTable->lookupTable(type.at(i)))
        {
            if(!tableList.contains(faultTable->table))
            {
                tableList.append(faultTable->table);
                qSort(tableList.begin(), tableList.end());

                index = tableList.indexOf(faultTable->table) * 8;
                udata.insert(index, faultTable->table);
                udata.insert(index + 1, 0);
                udata.insert(index + 2, 0);
                udata.insert(index + 3, 0);
                udata.insert(index + 4, 0);
                udata.insert(index + 5, 0);
                udata.insert(index + 6, 0);
                udata.insert(index + 7, 0);
            }

            index = udata.indexOf(faultTable->table) + faultTable->byte;
            if(index <= udata.size())
            {
                data = udata.at(index);
                data |= 1 << faultTable->bit;
                udata.replace(index, data);
            }
        }
    }

    qDebug() << hex << udata;
}

void Waring::dataSegment(QList<uchar> type)
{
    uchar data, index;
    QList<uchar> tableList;
    for(int i = 0; i < type.size(); i++)
    {
        if(waringTable->lookupTable(type.at(i)))
        {
            if(!tableList.contains(waringTable->table))
            {
                tableList.append(waringTable->table);
                qSort(tableList.begin(), tableList.end());

                index = tableList.indexOf(waringTable->table) * 8;
                udata.insert(index, 0);
                udata.insert(index + 1, 0);
                udata.insert(index + 2, 0);
                udata.insert(index + 3, 0);
                udata.insert(index + 4, 0);
                udata.insert(index + 5, 0);
                udata.insert(index + 6, 0);
                udata.insert(index + 7, 0);
            }

            index = udata.indexOf(waringTable->table) + waringTable->byte;
            if(index <= udata.size())
            {
                data = udata.at(index);
                data |= 1 << waringTable->bit;
                udata.replace(index, data);
            }
        }
    }

    qDebug() << hex << udata;
}

Realtime::~Realtime()
{
}

Management::Management(QObject *parent) :
    QObject(parent)
{
    pState = new Login(this);
}

Management::~Management()
{
    if(pState)
    {
        delete pState;
        pState = NULL;
    }
}
