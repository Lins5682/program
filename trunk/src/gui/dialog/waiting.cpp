#include "waiting.h"

Waiting::Waiting(QWidget *parent)
{
    widget = new QWidget(parent);
    parentwd = parent->width();
    parentht = parent->height();

    label_waitText = new QLabel(widget);
    Style style;
    style.setLabelStyle(label_waitText, 36, Style::blue, QFont::Bold);

    progressbar = new QProgressBar(widget);
    progressbar->setRange(0, 0);
    progressbar->resize(300, 22);
}

Waiting::~Waiting()
{
    if(!widget) { delete widget; widget = NULL;}
    if(!label_waitText) { delete label_waitText; label_waitText = NULL;}
    if(!progressbar) { delete progressbar; progressbar = NULL;}
}

void Waiting::setWaitText(const QString remind)
{
    label_waitText->setText(remind);

    label_waitText->adjustSize();
    int x = label_waitText->x();
    x +=  (label_waitText->width() - progressbar->width()) / 2;
    progressbar->move(x, progressbar->y() + 60);

    widget->adjustSize();
    int wd = parentwd - widget->width();
    int ht = parentht - widget->height();
    widget->move(wd / 2,ht / 2);
}

void Waiting::hide()
{
    widget->hide();
}

void Waiting::show()
{
    widget->show();
}
