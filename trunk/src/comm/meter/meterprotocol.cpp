#include "meterprotocol.h"
#include <QDebug>



#define PARA_MAX_SIZE            3


#define U8DATACAT(h1,h0,l1,l0)     (((quint32)(h1)*1000000)+((quint32)(h0)*10000)+((quint32)(l1)*100)+(l0))
//#define U8DATACAT(h,l)             (((quint16)(h)*100)+(l))


#define COMMON_OUT_TIME             5
MeterProtocol::MeterProtocol(const QString &name):
            Interface(CA_DLT645_2007, name)
{

}


void MeterProtocol::sendReadCmd(QByteArray addr,quint32 type)
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
        data.append(addr[i]) ;
    }
    data.append(0x68);
    data.append(0x11);
    data.append(0x04);

    di.d = type;
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
    quint8  i;
    quint8  e[4];
    quint32 var;
    QByteArray addr;
    union
    {
        quint32 d;
        struct
        {
            quint8 d[4];
        }byte;
    }di;


    if((quint8)data.at(4) != 0x68)return -1;

    if((quint8)data.at(data.size() - 1) != 0x16)return -1;
    for(i = 0; i < 6; i++)
    {
        addr[i] = data.at(5 + i);
    }
    if(i > 6)return -1;
    if((quint8)data.at(11) != 0x68)return -1;


    for(i = 0; i < 4; i++)
    {
        e[i] = 0;
    }
    if((quint8)data.at(12) == 0x91)
    {

        //长度没有校验
        for(i = 0; i < 4; i++)
        {
            di.byte.d[i] = data.at(14+i);
        }

        switch(di.d)
        {
            case DI_ENERGY:
            {
            for(i = 0; i < 4; i++)
            {
                e[i] = BCDTonaturalNumber((quint8)data.at(18 + i)-0x33);

            }
            e[3] &= 0x7f;
            break;
            }
            case DI_VOLTAGE:
            {
            for(i = 0; i < 2; i++)
            {
                 e[i] = BCDTonaturalNumber((quint8)data.at(18 + i)-0x33);
            }
            break;
            }
            case DI_CURRENT:
            {
            for(i = 0; i < 3; i++)
            {
                 e[i] = BCDTonaturalNumber((quint8)data.at(18 + i)-0x33);
            }
            e[2] &= 0x7f;
            break;
            }
            default:
            {
                return -1;
            }
        }
        var = U8DATACAT(e[3],e[2],e[1],e[0]);
    }
    else
    {
        return -1;
    }
    emit sendDataToDriver(name,addr,di.d,var);
    return 1;
}

void MeterProtocol::receiveFromRouting(const QByteArray &data)
{
    receiveHandle(data);
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
     return (((b>>4) & 0x0f)*10 + (b & 0x0f));
}
