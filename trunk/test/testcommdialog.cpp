#include "testcommdialog.h"
#include "can/interfacecan.h"
#include "serial/interfaceserial.h"
#include "htmu/htmu.h"
#include "cardreader/cardreader.h"
#include "routing.h"

testCommDialog::testCommDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

#define name "testCOMM"

void testCommInit(void)
{
    Error::output("start COMMUNICATION test ... [i]", name);

    // dialog
    testCommDialog *d = new testCommDialog();
    d ->show();

    // interface can
//    InterfaceCan *can0 = new InterfaceCan("can0");
    //InterfaceCan *can1 = new InterfaceCan("can1");
//    if(!can0 ->initialize()) Error::output("can0 open failed [x]", name);
    //if(!can1 ->initialize()) Error::output("can1 open failed [x]", name);

    // interface serial
    InterfaceSerial *sci1 = new InterfaceSerial("sci1", "/dev/ttyS5", Interface::SERIAL_BAUD_57600);
    //InterfaceSerial *sci2 = new InterfaceSerial("sci2", "/dev/ttyS2", Interface::SERIAL_BAUD_57600);
    if(!sci1 ->initialize()) Error::output("sci1 open failed [x]", name);
    //if(!sci2 ->initialize()) Error::output("sci2 open failed [x]", name);

    // htmu
//    HTMU *htmu0 = new HTMU("htmu0", 0);
    //HTMU *htmu1 = new HTMU("htmu1", 1);


//    if(!htmu0 ->initialize()) Error::output("htmu0 open failed [x]", name);
//    htmu0->setHtcuVersion(1001);
//    htmu0->setParameter(11,12,13,14);

//    //if(!htmu1 ->initialize()) Error::output("htmu1 open failed [x]", name);
//    htmu0 ->setState(0, 0, 0);

    // cardreader
//    CardReader *card = new CardReader();
//    if(!card->M1->initialize()) Error::output("cardreader open failed [x]", name);

    // canca
    CanCA *canca = new CanCA();
    if(!canca ->initialize()) Error::output("canca open failed [x]", name);

    // routing
    Routing *c = new Routing();
    // register CI

//    c ->registerCR(0, *can0);
//    c ->registerCR(1, *can0);
    c ->registerCR(2, *sci1);
//    c ->registerCR(3, *sci2);
    // register CA

//    c ->registerCR(1, *card->M1);
//    c ->registerCR(2, *htmu0);
    //c ->registerCR(3, *htmu1);
//    c ->registerCR(4, *canca);

    // rule

//    c ->setRuleCR(0, 2);
//    c ->setRuleCR(1, 4);
//    c ->setRuleCR(2, 1);
    //c ->setRuleCR(3, 1);
}

void CanCA::receiveFromRouting(QByteArray data)
{
    //Error::outputCan(data);
}
