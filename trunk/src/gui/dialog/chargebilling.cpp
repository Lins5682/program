#include "chargebilling.h"
#include "ui_chargebilling.h"

ChargeBilling::ChargeBilling(QWidget *parent) :
    Page(parent),
    ui(new Ui::ChargeBilling)
{
    ui->setupUi(this);
    hide();

    Style style;

    /*标题img,文字,提示语*/
    showLabelTitle("结算信息");

    /*设置参数提示Label*/
    style.setLabelStyle(ui->label_per_2);
    style.setLabelStyle(ui->label_KWh_3);
    style.setLabelStyle(ui->label_KWh_2);
    style.setLabelStyle(ui->label_per);
    style.setLabelStyle(ui->label_9);
    style.setLabelStyle(ui->label_8);
    style.setLabelStyle(ui->label_7);
    style.setLabelStyle(ui->label_11);
    style.setLabelStyle(ui->label_3);
    style.setLabelStyle(ui->label_6);
    style.setLabelStyle(ui->label_4);
    style.setLabelStyle(ui->label_2);
    style.setLabelStyle(ui->label_10);
    style.setLabelStyle(ui->label_11);
    style.setLabelStyle(ui->label_per);
    style.setLabelStyle(ui->label_12);

    /*设置参数显示lineEdit*/
    style.setLineEditStyle(ui->lineEdit_chargetimes);
    style.setLineEditStyle(ui->lineEdit_begintime);
    style.setLineEditStyle(ui->lineEdit_chargeelectricity);
    style.setLineEditStyle(ui->lineEdit_endelectricity);
    style.setLineEditStyle(ui->lineEdit_electricityfees);
    style.setLineEditStyle(ui->lineEdit_servicecharge);
    style.setLineEditStyle(ui->lineEdit_balance);
    style.setLineEditStyle(ui->lineEdit_beginSOC);
    style.setLineEditStyle(ui->lineEdit_endSOC);
    style.setLineEditStyle(ui->lineEdit_stopreason);

    /*确定按钮*/
    style.setButtonStyle(ui->pushButton);
}

ChargeBilling::~ChargeBilling()
{
    delete ui;
}

void ChargeBilling::on_pushButton_clicked()
{
    emit chargebillConfirm();
}
