#include "tip.h"
#include "ui_tip.h"


QString Tip::remindimg[] = { ":/pic/image/waring.png",
                                ":/pic/image/err.png",
                                ":/pic/image/ok.png"};

Tip::Tip(QWidget *parent) :
    Page(parent),
    ui(new Ui::Tip)
{
    createRemindLabel(this);
    ui->setupUi(this);
    hide();
}

Tip::~Tip()
{
    delete ui;
}

void Tip::createRemindLabel(QWidget *parent)
{

    labelImg = new QLabel(parent);
    labelImg->setGeometry(QRect(80, 130, 150, 65));

    labelRemind = new QLabel(parent);
    Style style;
    style.setLabelStyle(labelRemind, 36, Style::blue, QFont::Bold);
    labelRemind->setGeometry(QRect(80 + 80, 130, 700, 65));

}

void Tip::moveRemindPosition(int x, int y)
{
    labelImg->setGeometry(QRect(x, y, 150, 65));
    labelRemind->setGeometry(QRect(x + 80, y, 700, 65));
}

void Tip::setRemindImg(const RemindImg imgtype)
{
    QPixmap pixmap;
    imgtype > ok ? pixmap = remindimg[0] : pixmap = remindimg[imgtype];
    labelImg->setPixmap(QPixmap(pixmap));
}

void Tip::setRemindImg(const QString imgpath)
{
    labelImg->setPixmap(QPixmap(imgpath));
}

void Tip::setRemind(const QString remind)
{
    labelRemind->setText(remind);
}
