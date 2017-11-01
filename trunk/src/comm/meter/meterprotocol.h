#ifndef METERPROTOCOL_H
#define METERPROTOCOL_H

#include <QObject>
#include "routing.h"


#define Energy                   0
#define Voltage                  1
#define Current                  2

#define DI_ENERGY                0x00000000
#define DI_VOLTAGE               0x00010102
#define DI_CURRENT               0x00010202

class MeterProtocol : public Interface
{
    Q_OBJECT
public:

    explicit MeterProtocol(const QString &name);

private:

    union
    {
        quint32 d;
        struct
        {
            quint8 d[4];
        }byte;
    }DI;
    quint8 chkSum(QByteArray &data,quint8 start);
    quint8 BCDTonaturalNumber(quint8 b);

    int    receiveHandle(const QByteArray &data);
    void   setAddr(const QString &addr);

public:

    void sendReadCmd(QByteArray addr,quint32 type);

signals:

    void sendDataToDriver(QString & name,const QByteArray & addr,quint32 type, quint32 para);

public slots:

    void receiveFromRouting(const QByteArray &data);

};
#endif // METERPROTOCOL_H
