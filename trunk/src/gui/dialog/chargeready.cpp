#include "chargeready.h"
#include "ui_chargeready.h"

Chargeready::Chargeready(QWidget *parent) :
    Page(parent),
    ui(new Ui::Chargeready)
{
    ui->setupUi(this);
    hide();

    Style style;

    /*标题img,文字,提示语*/
    showLabelTitle("用户信息");

    /*设置参数提示Label*/
    style.setLabelStyle(ui->label_2);
    style.setLabelStyle(ui->label_3);
    style.setLabelStyle(ui->label_4);
    style.setLabelStyle(ui->label_6);
    style.setLabelStyle(ui->label_7);
    style.setLabelStyle(ui->label_8);
    style.setLabelStyle(ui->label_10);
    style.setLabelStyle(ui->label_11);

    /*设置参数显示lineEdit*/
    style.setLineEditStyle(ui->lineEdit_cardid);
    style.setLineEditStyle(ui->lineEdit_phone);
    style.setLineEditStyle(ui->lineEdit_cardbalance, Style::orange);
    style.setLineEditStyle(ui->lineEdit_serialnumber);
    style.setLineEditStyle(ui->lineEdit_accountbalance, Style::orange);
    style.setLineEditStyle(ui->lineEdit_elecprice);
    style.setLineEditStyle(ui->lineEdit_serviceprice);
    style.setLineEditStyle(ui->lineEdit_totalprice);


    style.setButtonStyle(ui->pushButton);
    style.setButtonStyle(ui->pushButton_2);
}

Chargeready::~Chargeready()
{
    delete ui;
}

void Chargeready::setLabelVariable1(const QString &s)
{
    ui->label_8->setText(s);
}

void Chargeready::setLabelVariable2(const QString &s)
{
//    ui->label_9->setText(s);
}

void Chargeready::setLabelVariable3(const QString &s)
{
    ui->label_10->setText(s);
}

//void Chargeready::on_DebugButton_clicked()
//{
//    emit clickdebugbuttion();
//}
void Chargeready::enter(void)
{
    show();
}
void Chargeready::exit(void)
{
    hide();
}

void Chargeready::on_pushButton_clicked()
{
    emit clickedCancleButton();
}

void Chargeready::on_pushButton_2_clicked()
{
    emit clickedConfirmButton();
}
