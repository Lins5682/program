#ifndef CHARGEBILLING_H
#define CHARGEBILLING_H

#include <QDialog>
#include <QTime>
#include "dialog/page.h"
#include "ui_chargebilling.h"

namespace Ui {
class ChargeBilling;
}

class ChargeBilling : public Page
{
    Q_OBJECT
    
public:
    explicit ChargeBilling(QWidget *parent = 0);
    ~ChargeBilling();

    void showSettlementInfo()
    {
        ui->lineEdit_chargetimes->setText(getChargeTimes().toString());

        ui->lineEdit_begintime->setText(getBeginTime().toString());

        ui->lineEdit_chargeelectricity->setText(QString::number(getElectricity()));

        ui->lineEdit_endelectricity->setText(QString::number(getEndElectricity()));

        ui->lineEdit_electricityfees->setText(QString::number(getElectricityfees()));

        ui->lineEdit_servicecharge->setText(QString::number(getServiceCharge()));

        ui->lineEdit_balance->setText(QString::number(getBalance()));

        ui->lineEdit_beginSOC->setText(QString::number(getBeginSOC()));

        ui->lineEdit_endSOC->setText(QString::number(getEndSOC()));

        ui->lineEdit_stopreason->setText(QString::number(getStopReason()));
    }

private:
    QTime getChargeTimes()
    {
        QTime time;
        time = QTime::currentTime();
        return time;
    }
    QTime getBeginTime()
    {
        QTime time;
        time = QTime::currentTime();
        return time;
    }
    double getElectricity()
    {
        double electricity;
        electricity = 123.1;
        return electricity;
    }
    double getEndElectricity()
    {
        double electricity;
        electricity = 230.2;
        return electricity;
    }

    double getElectricityfees()
    {
        double electricityfees;
        electricityfees = 750.1;
        return electricityfees;
    }

    double getServiceCharge()
    {
        double servicecharge;
        servicecharge = 750.1;
        return servicecharge;
    }

    double getBalance()
    {
        double balance;
        balance = 750.1;
        return balance;
    }

    uchar getBeginSOC()
    {
        uchar soc;
        soc = 30;
        return soc;
    }

    uchar getEndSOC()
    {
        uchar soc;
        soc = 30;
        return soc;
    }

    uchar getStopReason()
    {
        uchar stopcode;
        stopcode = 30;
        return stopcode;
    }

private:
    Ui::ChargeBilling *ui;
    QWidget *widget_welcome;

signals:
    void chargebillConfirm(void);

private slots:
    void on_pushButton_clicked();
};

#endif // CHARGEBILLING_H
