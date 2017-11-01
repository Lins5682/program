#ifndef FAULTDIALOG_H
#define FAULTDIALOG_H

#include <QDialog>
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QComboBox>
#include <QLineEdit>
#include "fault.h"
#include <QDebug>

namespace Ui {
class Faultdialog;
}

class Faultdialog;

class FaultMode : public QObject
{
    Q_OBJECT
public:
    FaultMode(Faultdialog *fd, int number) : faultdialog(fd), gunnumber(number)
    {
    }
    ~FaultMode()
    {
    }

    void pbtnFault(int type, bool ISFault);

signals:
    void sendToExternFault(int type, bool ISFault);
public slots:
    void showAddFault(int level, int type, QString msg);
    void showDeleteFault(int type);
private:
    Faultdialog *faultdialog;
    int gunnumber;
};

class Faultdialog : public QDialog
{
    Q_OBJECT
public:
    explicit Faultdialog(int number = 1, QWidget *parent = 0);
    ~Faultdialog();

private:
    void pbtnWork(bool ISFault);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public slots:
    void showAddFault(int gunnumber, int level, int type, QString msg);
    void showDeleteFault(int gunnumber, int type);

private:
    Ui::Faultdialog *ui;
    int gunnumber;
    QList<class Fault*> fault;
    QList<class FaultMode*> faultmode;
};

#endif // FAULTDIALOG_H
