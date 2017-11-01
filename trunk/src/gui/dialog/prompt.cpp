#include "prompt.h"

QString Prompt::remindimg[] = { ":/pic/image/waring.png",
                                ":/pic/image/err.png",
                                ":/pic/image/ok.png"};

Prompt::Prompt(QWidget *parent)
{
    createRemindLabel(parent);
}

Prompt::~Prompt()
{
    if(widget) { delete widget; widget = NULL; }
    if(label_img) { delete label_img; label_img = NULL; }
    if(label_remind) { delete label_remind; label_remind = NULL; }
}

void Prompt::createRemindLabel(QWidget *parent)
{
    widget = NULL;
    widget = new QWidget(parent);
    parentwd = parent->width();
    parentht = parent->height();

    label_img = NULL;
    label_img = new QLabel(widget);

    label_remind = NULL;
    label_remind = new QLabel(widget);
    Style style;
    style.setLabelStyle(label_remind, 36, Style::blue, QFont::Bold);
}

void Prompt::setRemindImg(const RemindImg imgtype)
{
    QPixmap pixmap;
    imgtype > ok ? pixmap = remindimg[0] : pixmap = remindimg[imgtype];
    label_img->setPixmap(QPixmap(pixmap));
}

void Prompt::setRemindImg(const QString imgpath)
{
    label_img->setPixmap(QPixmap(imgpath));
}

void Prompt::setRemindText(const QString remind)
{
    label_remind->setText(remind);
}

void Prompt::setRemind(const QString remind, const RemindImg imgtype)
{
    setRemindImg(imgtype);
    setRemindText(remind);

    label_img->adjustSize();
    label_remind->adjustSize();
    label_remind->move(label_img->x() + label_img->width() + 15, label_remind->y());

    widget->adjustSize();
    int wd = parentwd - widget->width();
    int ht = parentht - widget->height();
    widget->move(wd / 2,ht / 2);
}

void Prompt::show()
{
    widget->show();
}

void Prompt::hide()
{
    widget->hide();
}
