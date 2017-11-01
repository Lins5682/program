#include "mylabel.h"
#include <QTextCodec>

//TODO:
Mylabel::Mylabel(QWidget *parent):QLabel(parent)
{
    //pixmap1 = new QPixmap("F:/qyht/HTCU/hmi/image/key_return.png");
    //image = new QImage image("F:/qyht/HTCU/hmi/image/key_return.png");
    //this->setPixmap(QPixmap::fromImage(image));
    //setPixmap(*pixmap1);
    //show();

    this->setPixmap(QPixmap(":/pic/image/key_return.png"));
    //this->setPixmap(QPixmap("F:/qyht/HTCU/hmi/image/key_return.png"));

}

void Mylabel::setImage(const QString &s)
{

    this->setPixmap(QPixmap::fromImage(QImage(s)));
}
void Mylabel::setTextInImage(const QString &s)
{
    this->text = s;
    //show();
}

void Mylabel::paintEvent(QPaintEvent *event)
{
    QFont LabelFont;

    QLabel::paintEvent(event) ;
    QPainter painter(this) ;

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    LabelFont.setFamily("Microsoft YaHei");             //ÉèÖÃÑÅºÚ×ÖÌå
    this->setFont(LabelFont);
    painter.drawText(QPoint(5,40),text);
}

