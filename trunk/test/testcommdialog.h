#ifndef TESTCOMMDIALOG_H
#define TESTCOMMDIALOG_H

#include "ui_testcommdialog.h"
#include "interface.h"

class testCommDialog : public QDialog, private Ui::testCommDialog
{
    Q_OBJECT
    
public:
    explicit testCommDialog(QWidget *parent = 0);
};

class CanCA : public Interface
{
    Q_OBJECT
public:
    explicit CanCA() : Interface(CA_QYHT_MONITORING_UNIT, "CANCA") {}

public Q_SLOTS:
    void receiveFromRouting(QByteArray data);
};

extern void testCommInit(void);

#endif // TESTCOMMDIALOG_H
