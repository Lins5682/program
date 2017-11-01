#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QObject>
#include <QByteArray>
#include <QStringList>
#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>

class Management;
namespace managementspace
{
class State : public QObject
{
    Q_OBJECT
#define PACKETHEAD  0x0601
#define PACKETTAIL  0x0f02

public:
    explicit State(Management *m) : management(m)
    {
    }
    ~State()
    {
    }
    virtual void dataSegment()
    {
    }
    void handle();

protected:
    QList<uchar> fromString(QString str, uchar length, uchar fillchar = '0');

    template <typename T> QList<uchar> fromInt(const T udata);

    QByteArray fromList(QList<uchar> dlist);

private:
    u_int32_t getTimeStamp();

protected:
    Management      *management;
    QList<uchar>    udata;

signals:
    void write(QByteArray);
};

class Login : public State
{
public:
    explicit Login(Management *m) : State(m)
    {
    }
    ~Login();
    void dataSegment();
};

class StartCharge : public State
{
public:
    explicit StartCharge(Management *m) : State(m)
    {
    }
    ~StartCharge();
    void dataSegment();
private:
    uchar       gunnumber;      //启动枪口号
    uchar       mode;           //启动方式
    QString     accountNO;      //账号
    u_int32_t   balance;        //余额
    u_int32_t   prepaidamount;  //预付费金额
    u_int32_t   freeamount;     //免费金额
    u_int32_t   currentmeter;   //当前电表读数
};

class StopCharge : public State
{
public:
    explicit StopCharge(Management *m) : State(m)
    {
    }
    ~StopCharge();
    void dataSegment();
private:
    Management  *management;
    uchar       gunnumber;      //停止枪口号
    uchar       reason;         //停止方式
    QString     accountNO;      //账号
    u_int32_t   balance;        //余额
    u_int32_t   prepaidamount;  //预付费金额
    u_int32_t   freeamount;     //免费金额
    u_int32_t   currentmeter;   //当前电表读数
    u_int32_t   chargequantity; //已充电量
};

class Table
{
public:
    Table(uchar tb = 8, uchar bb = 4) : tableBit(tb), byteBit(bb)
    {
    }

    ~Table()
    {
        TableMap.clear();
    }

    void insertTable(uchar table, uchar byte, uchar bit)
    {
        TableMap << (table << tableBit | (byte << byteBit) | bit);
    }

    void insertTable(QString str)
    {
        QStringList strItem, strNumber;
        strItem = str.split("/");
        for(int i = 0; i < strItem.size(); i++)
        {
            strNumber = strItem.at(i).split(",");
            if(strNumber.size() < 3)
            {
                strNumber.clear();
                strNumber << QString::number(0x00) << QString::number(0x00) << QString::number(0x00);
            }
            insertTable(strNumber.at(0).toInt(), strNumber.at(1).toInt(), strNumber.at(2).toInt());
        }
    }

    bool lookupTable(uchar type)
    {
        if(type > TableMap.size()) return false;
        table = (TableMap.at(type - 1) >> tableBit) & 0x00ff;
        byte = (TableMap.at(type - 1) >> byteBit) & 0x000f;
        bit = TableMap.at(type - 1) & 0x000f;
        if((table == 0) && (byte == 0) && (bit == 0)) return false;
        return true;
    }

private:
    uchar tableBit;
    uchar byteBit;
    QList<u_int16_t> TableMap;

public:
    uchar table;    //表号
    uchar byte;     //字节序
    uchar bit;      //位号
};

class Fault : public State
{
public:
    explicit Fault(Management *m) : State(m)
    {
        /*bit15：8-表号；bit7：4-字节序；bit3：0-位号*/
        faultTable = new Table();
        QString str;
        str = "1, 1 / 1, 1, 1 / 1, 1, 2 / 1, 1, 3 / 1, 1, 4 / 1, 1, 5 / 1, 1, 6 / 1, 1, 7 / \
               1, 2, 0 / 1, 2, 1 / 1, 2, 2 / 1, 2, 3 / 1, 2, 4 / 1, 2, 5 / 1, 2, 6 / 1, 2, 7 / \
               1, 3, 0 / 1, 3, 1 / 1, 3, 2 / 1, 3, 3 / 1, 3, 4 / 1, 3, 5 / 1, 3, 6 / 1, 3, 7 / \
               1, 4, 0 / 1, 4, 1 / 1, 4, 2 / 1, 4, 3 / 1, 4, 4 / 1, 4, 5 / 1, 4, 6 / 1, 4, 7 / \
               1, 5, 0 / 1, 5, 1 / 1, 5, 2 / 1, 5, 3 / 1, 5, 4 / 1, 5, 5 / 1, 5, 6 / 1, 5, 7 / \
               1, 6, 0 / 1, 6, 1 / 1, 6, 2 / 1, 6, 3 / 1, 6, 4 / 1, 6, 5 / 1, 6, 6 / 1, 6, 7 / \
               1, 7, 0 / 1, 7, 1 / 1, 7, 2 / 1, 7, 3 / 1, 7, 4 / 1, 7, 5 / 1, 7, 6 / 1, 7, 7 / \
               2, 1, 0 / 2, 1, 1 / 2, 2, 2 / 2, 1, 3 / 2, 1, 4 / 2, 1, 5 / 2, 1, 6 / 2, 1, 7";
        faultTable->insertTable(str);
    }
    ~Fault();
    void dataSegment(QList<uchar> type);
private:
    Table *faultTable;
};

class Waring : public State
{
public:
    explicit Waring(Management *m) : State(m)
    {
        /*bit15：8-表号；bit7：4-字节序；bit3：0-位号*/
        waringTable = new Table();
        QString str;
        str = "1, 0, 0 / 1, 0, 1 / 1, 0, 2 / 1, 0, 3 / 1, 0, 4 / 1, 0, 5 / 1, 0, 6 / 1, 0, 7 / \
               1, 1, 0 / 1, 1, 1 / 1, 1, 2 / 1, 1, 3 / 1, 1, 4 / 1, 1, 5 / 1, 1, 6 / 1, 1, 7 / \
               1, 2, 0 / 1, 2, 1 / 1, 2, 2 / 1, 2, 3 / 1, 2, 4 / 1, 2, 5 / 1, 2, 6 / 1, 2, 7 / \
               1, 3, 0 / 1, 3, 1 / 1, 3, 2 / 1, 3, 3 / 1, 3, 4 / 1, 3, 5 / 1, 3, 6 / 1, 3, 7 / \
               1, 4, 0 / 1, 4, 1 / 1, 4, 2 / 1, 4, 3 / 1, 4, 4 / 1, 4, 5 / 1, 4, 6 / 1, 4, 7 / \
               1, 5, 0 / 1, 5, 1 / 1, 5, 2 / 1, 5, 3 / 1, 5, 4 / 1, 5, 5 / 1, 5, 6 / 1, 5, 7 / \
               1, 6, 0 / 1, 6, 1 / 1, 6, 2 / 1, 6, 3 / 1, 6, 4 / 1, 6, 5 / 1, 6, 6 / 1, 6, 7 / \
               1, 7, 0 / 1, 7, 1 / 1, 7, 2 / 1, 7, 3 / 1, 7, 4 / 1, 7, 5 / 1, 7, 6 / 1, 7, 7 / \
               2, 1, 0 / 2, 1, 1 / 2, 2, 2 / 2, 1, 3 / 2, 1, 4 / 2, 1, 5 / 2, 1, 6 / 2, 1, 7";
        waringTable->insertTable(str);
    }
    ~Waring();
    void dataSegment(QList<uchar> type);
private:
    Table *waringTable;
};

class Realtime : public State
{
public:
    explicit Realtime(Management *m) : State(m)
    {
    }
    ~Realtime();
    QList<uchar> dataSegment(QList<uchar> type);
};
}

using namespace managementspace;

class Management : public QObject
{
    Q_OBJECT
public:
    explicit Management(QObject *parent = 0);
    ~Management();

    QString initialize()
    {
        /*读数据库*/        
        if(true)
        {
            devicetype = 1;                         //设备类型
            ratedpower = 123456;                    //额定功率
            ratedvoltage = 400;                     //额定电压
            longitude = "123.456";                  //经度
            latitude = "123.012345678901234";       //纬度
                                                    //所属电站编号
                                                    //所属地区编号
                                                    //营运类型
                                                    //桩内编号
                                                    //充电枪信息
        }
        return "ok";
    }
 public slots:
    void handle()
    {
        pState->handle();
    }

public:
    u_int8_t    devicetype;     //设备类型
    u_int32_t   ratedpower;     //额定功率
    u_int16_t   ratedvoltage;   //额定电压
    QString     longitude;      //经纬度
    QString     latitude;       //经纬度
    QString     station;        //所属电站编号
    QString     arear;          //所属地区编号
    u_int8_t    servertype;     //营运类型
    u_int16_t   insidenumber;   //桩内编号
    /*充电枪信息*/
    struct guninformation
    {
        u_int8_t    number;     //枪号
        u_int16_t   parkspace;  //停车位号
        u_int8_t    type;       //充电枪接口类型
    };
    QList<guninformation> gun;   //充电枪信息


    State *pState;

signals:
    
public slots:
    
};

#endif // MANAGEMENT_H
