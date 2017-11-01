#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QPainter>


class Mylabel : public QLabel
{

    Q_OBJECT
private:

    QString text;
    QImage *image;
    QPixmap *pixmap1;
public:
    explicit Mylabel(QWidget *parent=0);
    void setImage(const QString &s);
    void setTextInImage(const QString &s);
    void paintEvent(QPaintEvent *event);
};

#endif // MYLABEL_H
