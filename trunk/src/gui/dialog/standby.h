#ifndef STANDBY_H
#define STANDBY_H

#include <QDialog>
#include <QLabel>
#include "dialog/page.h"
#include "dialog/code.h"


namespace Ui {
class Standby;
}

class Standby : public Page
{
    Q_OBJECT
    
public:
    explicit Standby(QWidget *parent = 0);
    ~Standby();
    
private:
    Ui::Standby *ui;
    QFrame *lab_swcard;
    QLabel *lab_remind;
    QRCode *qrcode;

public:
    void setlabelRemindText(const QString & s);

//signals:
//    void sendEditSignal(QWidget *);
//private slots:
//    void getEditFocus(QWidget *w);
};

#endif // STANDBY_H
