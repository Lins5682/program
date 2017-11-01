#ifndef MANAGEMENTTEST_H
#define MANAGEMENTTEST_H

#include <QDialog>
#include "tcpthread.h"
#include "management.h"

namespace Ui {
class ManagementTest;
}

class ManagementTest : public QDialog
{
    Q_OBJECT
    
public:
    explicit ManagementTest(QWidget *parent = 0);
    ~ManagementTest();
    
signals:
    void push(QByteArray);
    void management_handle();
private slots:
    void on_pushButton_clicked();

private:
    Ui::ManagementTest *ui;

    Management  *management;
    TCPClient   *tcpClient;
    TCPThread   *tcpThreadObj;
};

#endif // MANAGEMENTTEST_H
