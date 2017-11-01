#ifndef CARDREADER_H
#define CARDREADER_H

#include <QObject>
#include <QDebug>
#include "mifareone.h"
#include "routing.h"
#include "serial/interfaceserial.h"

class CardReader : public QObject
{
    Q_OBJECT
public:
    explicit CardReader(QObject *parent = 0);
    virtual ~CardReader();

    static CardReader* instance()
    {
        if(!instance_)
            instance_ = new CardReader();
        return instance_;
    }

    void initialize();

private:
    MifareOne*          M1;
    InterfaceSerial*    pInterfaceSerial;
    static CardReader*  instance_;
    Routing*            route;

public slots:
    void poll();
};
#endif // CARDREADER_H
