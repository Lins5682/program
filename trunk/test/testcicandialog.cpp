#include "testcicandialog.h"
#include <QDebug>
#include <QThread>

TestCICanDialog::TestCICanDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    sendBuf.resize(15);
    sendTimer0 = new QTimer(this);
    sendTimer1 = new QTimer(this);
    QObject::connect(sendTimer0, SIGNAL(timeout()),\
                     this, SLOT(send0()));
    QObject::connect(sendTimer1, SIGNAL(timeout()),\
                     this, SLOT(send1set()));
    sendTimer0 ->start(2000);
    sendTimer1 ->start(1000);
}

#define name    "testCAN"

void testCICanInit()
{
    Error::output("start CAN test ...", name);
    InterfaceCan *can0 = new InterfaceCan("can0");
    InterfaceCan *can1 = new InterfaceCan("can1");
    can0 ->initialize();
    can1 ->initialize();
    TestCICanDialog *cr = new TestCICanDialog();
    cr ->show();
    QObject::connect(cr, SIGNAL(sendToCan0(const QByteArray&)),\
                     can0, SLOT(receiveFromRouting(const QByteArray&)));
    QObject::connect(can0, SIGNAL(sendToRouting(QByteArray)),\
                     cr, SLOT(receiveFromCan(QByteArray)));
    QObject::connect(cr, SIGNAL(sendToCan1(const QByteArray&)),\
                     can1, SLOT(receiveFromRouting(const QByteArray&)));
    QObject::connect(can1, SIGNAL(sendToRouting(QByteArray)),\
                     cr, SLOT(receiveFromCan(QByteArray)));
    Error::output("ready to test.", name);
}

void TestCICanDialog::send0()
{
    static char i=0, j=0, k=0;
    InterfaceCan::setCanId(sendBuf, 0x0c003344);
    InterfaceCan::setCanData(sendBuf, 0x0c, 0,i,i,j,j,k,k);
    i+=4;
    j+=2;
    k++;
    Q_EMIT sendToCan0(sendBuf);
    Error::output("send can0 ok.", name);
}

void TestCICanDialog::send1()
{
    static char i=0, j=0, k=0;
    Interface::setCanId(sendBuf, 0x0c015566);
    Interface::setCanData(sendBuf, 0x0c, 1,i,i,j,j,k,k);
    i+=4;
    j+=2;
    k++;
    Q_EMIT sendToCan1(sendBuf);
    Error::output("send can1 ok.", name);
}

void TestCICanDialog::send1set()
{
    QObject::disconnect(sendTimer1,0,0,0);
    QObject::connect(sendTimer1, SIGNAL(timeout()),\
                     this, SLOT(send1()));
    sendTimer1 ->start(2000);
}

void TestCICanDialog::receiveFromCan(QByteArray data)
{
    if(!Interface::checkFrame(data, Interface::CAN))
    {
        Error::output("receive data type error.", name);
        return;
    }
    Error::outputCan(data, name);
}
