#ifndef TESTCISERIALDIALOG_H
#define TESTCISERIALDIALOG_H

#include "ui_testciserialdialog.h"
#include "serial/interfaceserial.h"

class TestCISerialDialog : public QDialog, private Ui::TestCISerialDialog
{
    Q_OBJECT
    
public:
    explicit TestCISerialDialog(QWidget *parent = 0);

signals:
    void send(QByteArray data);

public slots:
    void receive(QByteArray data);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_clear_clicked();
};

extern void testCISerialInit(void);

#endif // TESTCISERIALDIALOG_H
