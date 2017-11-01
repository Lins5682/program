#include "welcome.h"
#include "ui_welcome.h"

Welcome::Welcome(QWidget *parent) :
    Page(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);

    QPixmap pix(":/pic/image/shelf.png");
    ui->label_img->setFixedSize(pix.size());
    ui->label_img->setPixmap(pix);

    Style style;
    style.setLabelText(ui->label_remind, "请把充电枪归位，欢迎下次使用", 30, Style::blue);

    hide();
}

Welcome::~Welcome()
{
    delete ui;
}









