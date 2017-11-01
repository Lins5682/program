#ifndef USER_H
#define USER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QDateTime>

struct UserOrder
{
    QString     serialNumber;
    QString     consume;
    int         endSOC;
    int         startSOC;
    QDateTime   endTime;
    QDateTime   startTime;
};

struct UserBaseData
{
    QString     iccNums;
    QString     account;
    QString     balance;
};

struct UserRealTimeData
{
};

class User
{
public:
    explicit User(const QString&);
    ~User();
private:
    QString tlName;
};

#endif // USER_H
