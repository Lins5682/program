#include "chargeconfirm.h"
#include "ui_chargeconfirm.h"

Chargeconfirm::Chargeconfirm(QWidget *parent) :
    Page(parent),
    ui(new Ui::Chargeconfirm)
{
    ui->setupUi(this);
    hide();

    /*标题img,文字,提示语*/
    showLabelTitle("充电确认");
    initButtonStyle();

    prompt = new Prompt(this);
}

Chargeconfirm::~Chargeconfirm()
{
    delete ui;
}
void Chargeconfirm::initButtonStyle()
{
    Style style;
    style.setButtonStyle(ui->pushButton);
    style.setButtonStyle(ui->pushButton_2);
}

void Chargeconfirm::on_pushButton_clicked()
{
    emit clickedButton("cancle");
}

void Chargeconfirm::on_pushButton_2_clicked()
{
    emit clickedButton("confirm");
}
void Chargeconfirm::enter(void)
{
    show();
}
void Chargeconfirm::exit(void)
{
    hide();
}
