#include "ui_metertestdialog.h"
#include "metertestdialog.h"

#include <QDebug>


MeterTestDialog::MeterTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeterTestDialog)
{
    ui->setupUi(this);

    driver  = MeterDriver::meterDriver();

    ptl     = new MeterProtocol("ptl_DTL_645_0");
    ptl_1   = new MeterProtocol("ptl_DTL_645_1");


    driver->setMeterChannel(ptl);
    driver->setMeterChannel(ptl_1);

    connect(ptl, SIGNAL(sendToRouting(QByteArray)),
           this, SLOT(reciveCmd(QByteArray)));
    connect(this, SIGNAL(sendBaceData(QByteArray)),
            ptl, SLOT(receiveFromRouting(QByteArray)));
    connect(ptl_1, SIGNAL(sendToRouting(QByteArray)),
           this, SLOT(reciveCmd(QByteArray)));
    connect(this, SIGNAL(sendBaceData(QByteArray)),
            ptl_1, SLOT(receiveFromRouting(QByteArray)));



    QTimer *time = new QTimer(this);
    connect( time, SIGNAL(timeout()), this, SLOT(displayData()));
    time->start(1000);


}

MeterTestDialog::~MeterTestDialog()
{
    delete ui;
}

void MeterTestDialog::reciveCmd(QByteArray data)
{
    QString str;
    quint8 i;

    for(i = 0; i < data.size();i++)
    {
        str += QString::number((quint8)data.at(i),16);
    }
    ui->textEdit->append(str);
}

quint8 energyTestData[] = {
    0xfe,
    0xfe,
    0xfe,
    0xfe,
    0x68,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x68,
    0x91,
    0x04,

    0x00,
    0x00,
    0x00,
    0x00,

    0xCC,
    0xCB,
    0x33,
    0x33,

    0xaa,
    0x16,
};

void MeterTestDialog::on_pushButton_1_clicked()
{
    QByteArray data;
    quint8 i;
    int e;

    e = ui->comboBox->currentIndex();
    for(i = 0; i < 24; i ++)
    {
        data[i] = energyTestData[i];
    }
    if(e == 1)
    {
        data[10] = 0x07;
    }
    emit sendBaceData(data);
}

void MeterTestDialog:: displayData(void)
{
    if(!meter.isEmpty())
    {
        ui->lineEdit_3->setText(QString::number(meter[0]->getEnergy(),10));
    }
}

void MeterTestDialog::on_pushButton_clicked()
{
    QByteArray data;
    quint8 i;
    for(i = 0; i < 24; i ++)
    {
        data[i] = energyTestData[i];
    }
    data[10] = 0x07;
    emit sendBaceData(data);
}

void MeterTestDialog::on_pushButton_2_clicked()
{
    int e,i,a;
    bool ok;
    QString str;
    QByteArray meterAddr;

    e = ui->comboBox_2->currentText().toInt(&ok);
    qDebug()<< e;
    for(i = 0; i < 12; i+=2)
    {
        meterAddr[i/2] = ui->lineEdit_4->text().mid(i,2).toInt(&ok,16);
    }
    meter.append(new MeterDevice(meterAddr,e,1));
    ui->lineEdit_5->setText(QString::number(MeterDevice::getmeterDeviceNum(),10));
}
