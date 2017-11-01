#ifndef METERDRIVER_H
#define METERDRIVER_H

#include <QObject>
#include "meterprotocol.h"




struct MeterPara
{
    QByteArray addr;
    quint8     type;
    quint8     ch;
    bool       timeOut;
    quint32    energy;
    quint32    voltage;
    quint32    current;

};
struct MetercChannel
{
    MeterProtocol      *protocol;              //协议接口
    QList<MeterPara*>   deviceParaList;        //电表参数链表
    QByteArray          currentMeterAddr;      //当前电表地址
    quint8              currentMeterPos;       //当前电表偏移
    quint8              currentMeterPara;      //当前电表参数
    quint8              listsize;              //电表总数
    bool                mutex;                 //互锁信号
};


class MeterDriver : public QObject
{
    Q_OBJECT
public:
    explicit MeterDriver(QObject *parent = 0);
    ~MeterDriver();
    void sendCmdCh(quint8 ch);
    int  setMeterPara(MeterPara *p,quint32 ch);
    int  removeMeterPara(MeterPara *p);
    void setMeterProtocol(MeterProtocol * p);
    int setMeterChannel(MeterProtocol *p);

    static MeterDriver *meterDriver();
private:

    static MeterDriver *driver;
    QList<MetercChannel*>   meter;
    quint8                  meterListSize;         //通道总数
    QTimer                 *time;                  //数据更新周期

signals:


private slots:

    void deviceNext(void);
    void MeterTimeOut(void);
    void sendCtrl(void);
public slots:
    void paraRefresh(QString & name,const QByteArray & addr, quint32 type, quint32 para);

};
#endif // METERDRIVER_H
