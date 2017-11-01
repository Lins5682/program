#ifndef TESTCRDIALOG_H
#define TESTCRDIALOG_H

#include "ui_testcrdialog.h"
#include <QList>
#include "routing.h"

class TestInterface : public Interface
{
    Q_OBJECT
public:
    explicit TestInterface(Interface::Type type, const QString &name) :
        Interface(type, name) {}
    void emitSendToRouting(QByteArray data)
    {
        emit sendToRouting(data);
    }
    void emitSetConnect(bool isConnected)
    {
        emit setConnect(isConnected);
    }
};

class TestCRDialog : public QDialog, private Ui::TestCRDialog
{
    Q_OBJECT
    
public:
    explicit TestCRDialog(QWidget *parent = 0);

    QList<TestInterface*> testCI;
    QList<TestInterface*> testCA;

private slots:
    void on_pushButtonCI_clicked();
    void on_pushButtonCA_clicked();
    void on_pushButtonConnect_clicked();
    void on_pushButtonDisconnect_clicked();
};

extern void testCRInit(void);

#endif // TESTCRDIALOG_H
