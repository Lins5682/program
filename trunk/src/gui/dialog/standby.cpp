#include "standby.h"
#include "ui_standby.h"
#include <QTextCodec>


Standby::Standby(QWidget *parent) :
    Page(parent),
    ui(new Ui::Standby)
{
    ui->setupUi(this);
    hide();

    Style style;

    QByteArray text("亨通新能源|充电大亨");
    qrcode  = new QRCode(text, QSize(150, 150),this);
    qrcode->setGeometry(QRect(150, 100, 150, 150));
    qrcode->setMargin (10);
    qrcode->show ();

    /*二维码img*/
//    ui->label_qrcodeimg->setPixmap(QPixmap(":/pic/image/title.png"));

    /*刷卡img*/
    ui->label_scimg->setPixmap(QPixmap(":/pic/image/swingcard.png"));

    /*提示语*/
    style.setLabelText(ui->label_remind, "请 扫 码 充 电 或 刷 卡 充 电", 19, Style::blue);
}

Standby::~Standby()
{
    delete ui;
}

void Standby::setlabelRemindText(const QString & s)
{
     ui->label_remind->setText(s);
}
