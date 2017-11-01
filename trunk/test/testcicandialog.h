#ifndef TESTCICANDIALOG_H
#define TESTCICANDIALOG_H

#include "ui_testcicandialog.h"
#include "can/interfacecan.h"
#include <QTimer>

class TestCICanDialog : public QDialog, private Ui::TestCICanDialog
{
    Q_OBJECT
    
public:
    explicit TestCICanDialog(QWidget *parent = 0);

Q_SIGNALS:
    void sendToCan0(const QByteArray &data);
    void sendToCan1(const QByteArray &data);

public Q_SLOTS:
    void receiveFromCan(QByteArray data);
    void send0();
    void send1();
    void send1set();
private:
    QTimer *sendTimer0;
    QTimer *sendTimer1;
    QByteArray sendBuf;
};

extern void testCICanInit();

#endif // TESTCICANDIALOG_H
