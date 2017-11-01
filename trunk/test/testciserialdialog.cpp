#include "testciserialdialog.h"
#include <QDebug>

TestCISerialDialog::TestCISerialDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void TestCISerialDialog::receive(QByteArray data)
{
    textBrowser ->insertPlainText(data + "\n");
    textBrowser ->moveCursor(QTextCursor::End);
}

void testCISerialInit(void)
{
    qDebug() << "TEST<SERIAL>: start SERIAL test ...";
    // interface
    InterfaceSerial *interface = new InterfaceSerial("uart1", "/dev/ttyS1");
    interface ->initialize();
    // routing port
    TestCISerialDialog *routingPort = new TestCISerialDialog();
    routingPort ->show();
    // CR --> CI
    QObject::connect(routingPort, SIGNAL(send(QByteArray)), interface, SLOT(receiveFromRouting(QByteArray)));
    // CI --> CR
    QObject::connect(interface, SIGNAL(sendToRouting(QByteArray)), routingPort, SLOT(receive(QByteArray)));
}

void TestCISerialDialog::on_pushButton_clicked()
{
    static QByteArray data;
    data = lineEdit ->text().toAscii();
    Interface::setStreamFrame(data);
    emit send(data);
}

void TestCISerialDialog::on_pushButton_clear_clicked()
{
    textBrowser ->clear();
}
