#ifndef MIFAREONE_H
#define MIFAREONE_H

#include <QByteArray>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include "interface.h"

class MifareOne : public Interface
{
    Q_OBJECT
public:
    explicit MifareOne(Type type = Interface::CA_SGCC_CARD_READER,
                  const QString &name = "cardreader",
                  QObject *parent = 0)
        : Interface(type, name, parent)
    {
        QList<uchar> key;
        key << 0x06 << 0x18 << 0x20 << 0x31 << 0x41 << 0x50;
        setkeyA(key);
        key.clear();
        key << 0x06 << 0x18 << 0x20 << 0x31 << 0x41 << 0x50;
        setkeyB(key);
    }
    virtual ~MifareOne()
    {
    }

private:
    enum ControlByte
    {
        START_BYTE = 0x02,      //起始字节
        END_BYTE = 0x03         //结束字节
    };

    struct Framestructure
    {
        uchar startbyte;        //起始字节
        u_int16_t length;       //数据域长度
        QList<uchar> appdata;   //数据域
        uchar bcc;              //bcc校验
        uchar endbyte;          //结束字节
    };

    bool ISReadEnd;             //接收完成标识
    QList<uchar> data;          //发送数据
    QByteArray readbuf;         //接收缓存
    u_int16_t status;           //接收完成帧处理结果状态
    struct Framestructure read; //接收数据

    QByteArray uid;
    QString type;

protected:
    QList<uchar> key_a;
    QList<uchar> key_b;

public:
    //设置A密钥
    void setkeyA(QList<uchar> key);

    //设置B密钥
    void setkeyB(QList<uchar> key);

    //读卡号
    bool readCardNumber(QString &number);

private:
    bool handle(int delay = 1000);

    //bcc校验
    uchar BCC(QList<uchar> data);

    //设置波特率
    bool setBaudrate(int baud);

    //查看读卡器版本信息
    QString readVersion();

    //蜂鸣器控制 - @@@不具备
    bool controlBuzzer(u_int16_t delaytime, u_int8_t times);

    //LED状态控制 - @@@不具备
    bool controlLED(bool greenstate = false, bool redstate = false);

    //打开射频 - @@@未验证
    bool openRadioFrequency();

    //关闭射频 - @@@不成功
    bool closeRadioFrequency();

    //接触式卡上电 - @@@未实现
    void poweronContactCard();

    //接触式卡下电 - @@@未实现
    void poweroffContactCard();

    //激活非接触式卡
    QString activateContactlessCard(u_int16_t delaytime = 0);

    //应用层传输命令 - 未验证
    void appTransmission();

    //载入密钥 - 验证存疑
    bool loadKey(int sector, QString keytype, QList<uchar> key);

    //模块密钥认证 - @@@指令作用不明
    bool authKey(int block, QString keytype);

    //M1卡认证
    QString authM1(int block, QString keytype, QList<uchar> key);

    //读块数据
    bool readBlock(int block, QList<uchar> &blockdata);

    //写块数据
    bool writeBlock(int block, QList<uchar> blockdata);

    //加值
    bool addValue(int block, int value);

    //减值
    bool subtractValue(int block, int value);

    //传输
    bool transmission(int block);

    //恢复 - @@@未验证
    bool recovery(int block);

    //更改A密码
    bool changeKeyA(int sector, QList<uchar> keyA);

    //块数据初始化
    bool initBlock(int block, QList<uchar> blockdata);

    //接触式卡弹卡 - @@@未验证
    QString popContactCard(u_int16_t delaytime, int cardno = 0x00);

public slots:
    void receiveFromRouting(QByteArray data);
};


#endif // MIFAREONE_H
