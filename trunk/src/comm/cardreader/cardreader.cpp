#include "cardreader.h"
#include <unistd.h>
#include <QStringList>
#include <QEventLoop>

CardReader* CardReader::instance_ = 0;
CardReader::CardReader(QObject *parent)
{
    M1 = new MifareOne();
    initialize();
}

CardReader::~CardReader()
{
    if(M1)
    {
        delete M1;
        M1 = NULL;
    }

    if(pInterfaceSerial)
    {
        delete pInterfaceSerial;
        pInterfaceSerial = NULL;
    }

    if(route)
    {
        delete route;
        route = NULL;
    }
}

void CardReader::initialize()
{
    pInterfaceSerial = new InterfaceSerial("icCard", "/dev/ttyS5", Interface::SERIAL_BAUD_57600);
    if(!pInterfaceSerial->initialize())
    {
        qWarning() << "It's failed to initialize icCardInterface!";
    }
    if(!M1->initialize())
    {
        qWarning() << "It's failed to initialize the M1 of icCardReader!";
    }

    Routing::instance()->registerCR(0, *pInterfaceSerial);
    Routing::instance()->registerCR(0, *M1);
    Routing::instance()->setRuleCR(0, 0);
}

void CardReader::poll()
{
    QString number;
    QEventLoop loop;

    while(1)
    {
        number.clear();
        if(M1->readCardNumber(number))
        {
            qDebug() << QTime::currentTime() << number;
        }
//        qDebug() << QTime::currentTime() << number;

        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }
}
