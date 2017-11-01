#ifndef CHARGEINFO_H
#define CHARGEINFO_H

#include <QDialog>
#include <QTimer>
#include <QLineEdit>
#include <QTime>
#include <stdint.h>
#include "ui_chargeinfo.h"
#include "dialog/page.h"

namespace Ui {
class ChargeInfo;
}

class ChargeInfo : public Page
{
    Q_OBJECT
    
public:
    explicit ChargeInfo(QWidget* parent);
    ~ChargeInfo();
    
private:
    Ui::ChargeInfo *ui;
    QTimer *refreshTimer;

public:
    virtual void enter(void);
    virtual void exit(void);

private:
    QString getCardid()
    {
        QString cardid;
        cardid = "1234567890123456";
        cardid.replace(4, 8, " **** **** ");
        return cardid;
    }
    QTime getBeginTime()
    {
        QTime begin;
        begin = QTime::currentTime();
        return begin;
    }
    QTime getTotalTimes()
    {
        QTime total;
        total = QTime::currentTime();
        return total;
    }
    double getMoney()
    {
        double money;
        money = 123.45;
        return money;
    }
    double getVoltage()
    {
        double voltage;
        voltage = 750.1;
        return voltage;
    }
    double getCurrent()
    {
        double current;
        current = 160.2;
        return current;
    }
    double getElectricity()
    {
        double electricity;
        electricity = 123.1;
        return electricity;
    }
    uchar getSOC()
    {
        uchar soc;
        soc = 30;
        return soc;
    }

private slots:
    void updateData();
};

#endif // CHARGEINFO_H

