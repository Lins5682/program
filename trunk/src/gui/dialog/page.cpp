#include "page.h"

Page::Page(QWidget *parent) :
     QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    label_img = NULL;
    label_title = NULL;
    label_remind = NULL;
    label_cdsec = NULL;
    countdown = NULL;

    /*设置背景*/
    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/image/content.png")));
    setPalette(palette);

    setGeometry(QRect(21, 65, 762, 383));
}

Page::~Page()
{
    DELETE(label_img);
    DELETE(label_title);
    DELETE(label_remind);
    DELETE(label_cdsec);
    DELETE(countdown);
}

void Page::createLabelTitle()
{
    Style style;
    int x, y;
    x = 25;
    y = 35;

    /*标题img*/
    if(label_img == NULL)
    {
        label_img = new QLabel(this);
        label_img->setGeometry(QRect(x, y, 9, 31));
    }

    /*标题文字*/
    if(label_title == NULL)
    {
        label_title = new QLabel(this);
        style.setLabelStyle(label_title, 31, Style::blue, QFont::Bold);
        label_title->setGeometry(QRect(x + 20, y, 150, 31));
    }

    /*标题提示语*/
    if(label_remind == NULL)
    {
        label_remind = new QLabel(this);
        style.setLabelStyle(label_remind, 22, Style::green, QFont::Bold);
        label_remind->setGeometry(QRect(x + 20 + 150, y + 3, 270, 31));
    }
}

void Page::showLabelTitle(const char *title, const char *remind)
{
    createLabelTitle();
    label_img->setPixmap(QPixmap(":/pic/image/title.png"));
    label_title->setText(QString::fromUtf8(title));
    label_remind->setText(QString::fromUtf8(remind));
}

void Page::showLabelTitle()
{
    if(label_img != NULL) label_img->show();

    if(label_title != NULL) label_title->show();

    if(label_remind != NULL)  label_remind->show();
}

void Page::hideLabelTitle()
{
    if(label_img != NULL) label_img->hide();

    if(label_title != NULL) label_title->hide();

    if(label_remind != NULL)  label_remind->hide();
}

void Page::createCountdown(int sec)
{
    if(countdown == NULL)
    {
        countdown = new QTimer(this);
    }
    countdown->setSingleShot(true);
    countdown->setInterval(1*1000);
    countdown_sec = sec;

    connect(countdown, SIGNAL(timeout()), this, SLOT(showCountdownSec()));

    if(label_cdsec == NULL)
    {
        label_cdsec = new QLabel(this);
        label_cdsec->setGeometry(QRect(715, 20, 20, 30));
        Style style;
        style.setLabelStyle(label_cdsec, 18, Style::black);
    }

    countdown->start();
}

void Page::showCountdownSec()
{
    countdown_sec--;
    label_cdsec->setText(QString::number(countdown_sec));

    if(countdown_sec >= 0)
    {
        countdown->start();
    }
    else
    {
        delete countdown;
        countdown = NULL;

        delete label_cdsec;
        label_cdsec = NULL;
    }
}
