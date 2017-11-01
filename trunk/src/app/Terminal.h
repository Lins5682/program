#ifndef TERMINAL_H
#define TERMINAL_H

#include "User.h"
#include "can/interfacecan.h"
#include "htmu/htmu.h"
#include "routing.h"
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QDebug>

class Terminal : public QObject
{
    Q_OBJECT
public:
    enum Status {
        None,
        Activated,
        Registered,
        Started,
        Terminated
    };

    explicit Terminal(const QString& tlName, int tlNumber = 0, int version = 1202, int serial = 0, const QString& code = "001");
    ~Terminal();

    void execute();
    void setParameters();

    QString name()
    {
        return name_;
    }

    /* TODO: Change the name of function.*/
    void handleCard(const QString& cardNum)
    {
        if (current == None)
        {
            /*TODO: Calloc a user with cardNum, and add the user to userManager*/
            emit (statusChanged(current = Activated));
        }

        if (current == Started)
        {
            /*
             * TODO:
             * 1. Judge the number of card whether is the same as user via userManager.
             * 2. Stop HTMU
            */
        }
    }

signals:
    void start();
    void stop();
    void statusChanged(int);
    void error(int, const QString&);

public slots:
    void started();
    void stoped();

private:
    QString name_;
    HTMU exec;
    static InterfaceCan *intanceCan0;
    User* user;
    Status current;
    int id;    
    QThread courseCtrl;    // 终端运行线程管理；
};

#endif // TERMINAL_H
