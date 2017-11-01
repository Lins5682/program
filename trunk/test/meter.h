#ifndef METER_H
#define METER_H

#include <QObject>
#include "routing.h"

class MeterProtocol : public Interface
{
    Q_OBJECT
public:

    explicit MeterProtocol(Interface::Type type, uchar channel, const QString &name);

private:

    union
    {
        quint32 d;
        struct
        {
            quint8 d[4];
        }byte;
    }DI;
    quint8 stateCnt;
    quint8 stateFlag;
    quint8 timeOut;
    quint8 addr[6];

    void   state(void);
    quint8 chkSum(QByteArray &data,quint8 start);
    quint8 BCDTonaturalNumber(quint8 b);

    int    receiveHandle(const QByteArray &data);
    void   setAddr(const QString &addr);

public:

    quint32 energy;
    quint32 voltage;
    quint32 current;

    void   sendReadCmd(uchar *addr,quint8 type);

signals:

    void sendDataToDriver(const QString & addr,quint32 type, quint32 para);

public slots:
    void receiveFromRouting(const QByteArray &data);
    
};


#define MeterParaTypeSize   3

struct MeterPara
{
    QString addr;
    bool    timeOut;
    quint32 energy;
    quint32 voltage;
    quint32 current;
};


class MeterDriver : public QObject
{
    Q_OBJECT
public:
    explicit MeterDriver(QObject *parent = 0);
    void setMeterPara(MeterPara* p);
    void setMeterProtocol(MeterProtocol * p);
private:


    MeterProtocol *protocol;
    QList<MeterPara*>  deviceParaList;

    QString currentAddr;
    quint8  currentPos;
    bool    mutex;

    void sendCtrl(void);
signals:

    void paraRead(uchar *addr,quint8 type);

private slots:

    void paraRefresh(const QString & addr, quint8 type, quint32 para);
    void MeterTimeOut(void);

};

class MeterDevice
{
public:
    explicit MeterDevice(const QString &addr);
    ~MeterDevice();

    QString   getAddr(void){return para.addr;}
    quint32   getEnergy(void){return para.energy;}
    quint32   getVoltage(void){return para.voltage;}
    quint32   getCurrent(void){return para.current;}

    static MeterDriver *driver;


private :

    MeterPara para;
};



#endif // METER_H
