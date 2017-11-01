#ifndef METERTESTDIALOG_H
#define METERTESTDIALOG_H

#include <QDialog>
#include "routing.h"
#include "meter/meterdevice.h"
#include "meter/meterprotocol.h"
#include "meter/meterdriver.h"
#include "meter/meterdevice.h"

namespace Ui {
class MeterTestDialog;
}

class MeterTestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MeterTestDialog(QWidget *parent = 0);
    ~MeterTestDialog();
    
private:
    Ui::MeterTestDialog *ui;
    QList<MeterDevice*> meter;
    MeterProtocol * ptl;
    MeterProtocol * ptl_1;
    MeterDriver   * driver;
signals:
    void sendBaceData(const QByteArray &data);

private slots:
    void reciveCmd(QByteArray data); 
    void on_pushButton_1_clicked();
    void displayData();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // METERTESTDIALOG_H
