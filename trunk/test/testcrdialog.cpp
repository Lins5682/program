#include "testcrdialog.h"
#include "error.h"
#include <QDebug>

TestCRDialog::TestCRDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

#define name    "testROUTING"

void TestCRDialog::on_pushButtonCI_clicked()
{
    bool ok;
    static QByteArray d;
    d.resize(21);
    uchar interfaceNumber = lineEditCI ->text().toInt(&ok, 10);
    if(interfaceNumber <= ROUTING_PORT_MAX) emit testCI.at(interfaceNumber) ->emitSendToRouting(d);
    else Error::output("CI LineEdit error.", name);
}

void TestCRDialog::on_pushButtonCA_clicked()
{
    bool ok;
    static QByteArray d;
    d.resize(12);
    uchar interfaceNumber = lineEditCA ->text().toInt(&ok, 10);
    if(interfaceNumber <= ROUTING_PORT_MAX) emit testCA.at(interfaceNumber) ->emitSendToRouting(d);
    else Error::output("CA LineEdit error.", name);
}

void TestCRDialog::on_pushButtonConnect_clicked()
{
    bool ok;
    uchar interfaceNumber = lineEditCI ->text().toInt(&ok, 10);
    if(interfaceNumber <= ROUTING_PORT_MAX)
    {
        testCI.at(interfaceNumber) ->connectedFlag = true;
        emit testCI.at(interfaceNumber) ->emitSetConnect(true);
    }
    else Error::output("CI LineEdit error.", name);
}

void TestCRDialog::on_pushButtonDisconnect_clicked()
{
    bool ok;
    uchar interfaceNumber = lineEditCI ->text().toInt(&ok, 10);
    if(interfaceNumber <= ROUTING_PORT_MAX)
    {
        testCI.at(interfaceNumber) ->connectedFlag = false;
        emit testCI.at(interfaceNumber) ->emitSetConnect(false);
    }
    else Error::output("CI LineEdit error.", name);
}

void testCRInit(void)
{
    Error::output("start ROUTING test ...", name);
    TestCRDialog *d = new TestCRDialog();
    d ->show();
    Routing *c = new Routing();

    uchar interfaceNumber = 0;
    d ->testCI.append(new TestInterface(Interface::CI_SERIAL, "uart0"));
    c ->registerCR(interfaceNumber, *d ->testCI.at(interfaceNumber));

    interfaceNumber = 1;
    d ->testCI.append(new TestInterface(Interface::CI_SERIAL, "uart1"));
    c ->registerCR(interfaceNumber, *d ->testCI.at(interfaceNumber));

    interfaceNumber = 2;
    d ->testCI.append(new TestInterface(Interface::CI_SERIAL, "uart2"));
    c ->registerCR(interfaceNumber, *d ->testCI.at(interfaceNumber));

    interfaceNumber = 3;
    d ->testCI.append(new TestInterface(Interface::CI_CAN, "can0"));
    c ->registerCR(interfaceNumber, *d ->testCI.at(interfaceNumber));

    interfaceNumber = 4;
    d ->testCI.append(new TestInterface(Interface::CI_CAN, "can1"));
    c ->registerCR(interfaceNumber, *d ->testCI.at(interfaceNumber));

    interfaceNumber = 5;
    d ->testCI.append(new TestInterface(Interface::CI_TCP, "tcp0"));
    c ->registerCR(interfaceNumber, *d ->testCI.at(interfaceNumber));

    // CA

    interfaceNumber = 0;
    d ->testCA.append(new TestInterface(Interface::CA_DLT645_2007, "dlt6450"));
    c ->registerCR(interfaceNumber, *d ->testCA.at(interfaceNumber));

    interfaceNumber = 1;
    d ->testCA.append(new TestInterface(Interface::CA_DLT645_2007, "dlt6451"));
    c ->registerCR(interfaceNumber, *d ->testCA.at(interfaceNumber));

    interfaceNumber = 2;
    d ->testCA.append(new TestInterface(Interface::CA_QYHT_MONITORING_UNIT, "qyhtmu0"));
    c ->registerCR( interfaceNumber, *d ->testCA.at(interfaceNumber));

    interfaceNumber = 3;
    d ->testCA.append(new TestInterface(Interface::CA_QYHT_MONITORING_UNIT, "qyhtmu1"));
    c ->registerCR(interfaceNumber, *d ->testCA.at(interfaceNumber));

    interfaceNumber = 4;
    d ->testCA.append(new TestInterface(Interface::CA_SGCC_CARD_READER, "sgcccr0"));
    c ->registerCR(interfaceNumber, *d ->testCA.at(interfaceNumber));

    interfaceNumber = 5;
    d ->testCA.append(new TestInterface(Interface::CA_SGCC_CARD_READER, "sgcccr1"));
    c ->registerCR(interfaceNumber, *d ->testCA.at(interfaceNumber));

    c ->registerCR(interfaceNumber, *d ->testCA.at(interfaceNumber), false);

    // RULE

//    c ->setRuleCR(true, 0, 2);
//    c ->setRuleCR(true, 1, 5);
//    c ->setRuleCR(true, 2, 4);
//    c ->setRuleCR(true, 3, 0);
//    c ->setRuleCR(true, 4, 3);

    c ->setRuleCR(0, 2);
    c ->setRuleCR(1, 5);
    c ->setRuleCR(2, 4);
    c ->setRuleCR(3, 0);
    c ->setRuleCR(4, 3);

    Error::output("init ok.", name);

}
