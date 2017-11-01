#include "managementtest.h"
#include "ui_managementtest.h"

ManagementTest::ManagementTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagementTest)
{
    ui->setupUi(this);

    tcpClient = new TCPClient("192.168.0.7", 8080);
    tcpClient->open();
    tcpThreadObj = new TCPThread(tcpClient);

    management = new Management();
    management->initialize();
    connect(management->pState, SIGNAL(write(QByteArray)), tcpClient, SLOT(pushInBuffer(QByteArray)));
    management->handle();

    connect(this, SIGNAL(push(QByteArray)), tcpClient, SLOT(pushInBuffer(QByteArray)));
    connect(this, SIGNAL(management_handle()), management, SLOT(handle()));

}

ManagementTest::~ManagementTest()
{
    delete ui;
}

void ManagementTest::on_pushButton_clicked()
{
    QString t = ui->lineEdit->text();
    QByteArray a;
    static uchar i = 0;
    a.append(QString::number(i++));
    a.append(t);
    a.append('\n');
//    emit push(a);
    emit management_handle();
}
