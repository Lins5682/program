#ifndef TIP_H
#define TIP_H

#include "dialog/page.h"
#include <QDialog>

namespace Ui {
class Tip;
}

class Tip : public  Page
{
    Q_OBJECT
    
public:
    explicit Tip(QWidget *parent = 0);
    ~Tip();

    enum RemindImg
    {
        waring,
        err,
        ok
    };
    
private:
    Ui::Tip *ui;
    QLabel  *labelImg;
    QLabel  *labelRemind;

    void createRemindLabel(QWidget *parent);

    static QString remindimg[3];


public:
    void setRemindImg(const RemindImg imgtype);
    void setRemindImg(const QString imgpath);
    void setRemind(const QString remind);
    void moveRemindPosition(int x, int y);

};

#endif // TIP_H
