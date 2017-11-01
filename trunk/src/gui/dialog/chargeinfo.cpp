#include "chargeinfo.h"
#include "mylabel.h"

ChargeInfo::ChargeInfo(QWidget* parent = 0) :
    Page(parent),
    ui(new Ui::ChargeInfo)
{
    ui->setupUi(this);
    hide();

    Style style;

    /*标题img,文字,提示语*/
    showLabelTitle("充电信息", "请刷卡停机");

    /*设置参数提示Label*/
    style.setLabelStyle(ui->label_V);
    style.setLabelStyle(ui->label_A);
    style.setLabelStyle(ui->label_KWh);
    style.setLabelStyle(ui->label_per);
    style.setLabelStyle(ui->label_9);
    style.setLabelStyle(ui->label_8);
    style.setLabelStyle(ui->label_7);
    style.setLabelStyle(ui->label_11);
    style.setLabelStyle(ui->label_3);
    style.setLabelStyle(ui->label_6);
    style.setLabelStyle(ui->label_4);
    style.setLabelStyle(ui->label_2);

    /*设置参数显示lineEdit*/
    style.setLineEditStyle(ui->lineEdit_cardid);
    style.setLineEditStyle(ui->lineEdit_begintime);
    style.setLineEditStyle(ui->lineEdit_totaltimes);
    style.setLineEditStyle(ui->lineEdit_money);
    style.setLineEditStyle(ui->lineEdit_voltage);
    style.setLineEditStyle(ui->lineEdit_current);
    style.setLineEditStyle(ui->lineEdit_electricity);
    style.setLineEditStyle(ui->lineEdit_SOC);

    /*扩展按钮*/
    style.setButtonStyle(ui->pushButton_right, ":/pic/image/right.png");
    ui->pushButton_right->raise();

    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(updateData()));
}

ChargeInfo::~ChargeInfo()
{
    delete ui;
    if(refreshTimer != NULL)
    {
        delete refreshTimer;
        refreshTimer = NULL;
    }
}

void ChargeInfo::enter(void)
{
    refreshTimer->start(400);
    show();
}

void ChargeInfo::exit(void)
{
    refreshTimer->stop();
    hide();    
}

void ChargeInfo::updateData()
{
    ui->lineEdit_cardid->setText(getCardid());

    ui->lineEdit_begintime->setText(getBeginTime().toString());

    ui->lineEdit_totaltimes->setText(getTotalTimes().toString());

    ui->lineEdit_money->setText(QString::number(getMoney()));

    ui->lineEdit_voltage->setText(QString::number(getVoltage()));

    ui->lineEdit_current->setText(QString::number(getCurrent()));

    ui->lineEdit_electricity->setText(QString::number(getElectricity()));

    ui->lineEdit_SOC->setText(QString::number(getSOC()));
}
