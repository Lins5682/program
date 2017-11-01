#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QTimer>
#include "error.h"

/*
 * 字节流定义                             time TYPE
 * ----------------------------------------------------------------------------
 *      0   1   2   3   4   5...    12  Last 2-1Byte
 *      -----------------------------------------------------------------------
 *      R   R   ID0 ID1 LEN D0...   D7  CAN standard
 *      ID0 ID1 ID2 ID3 LEN D0...   D7  CAN extended
 *      P   PF  PS  SA  LEN D0...   D7  CAN j1939-21
 *      data...                         STREAM
 * ------------------------------
 *      R, reversed:    0
 *      LEN, length:    0-8
 */

class ErrorRecode;

// 通信接口和通信应用基类
class Interface : public QObject
{
    Q_OBJECT
public:
    // 类型定义
    enum Type
    {
        CI_SERIAL = 0x10,
        CI_CAN = 0x20,
        CI_TCP = 0x30,
        CA_DLT645_2007 = 0xa0,
        CA_SGCC_CARD_READER = 0xa1,
        CA_QYHT_MONITORING_UNIT = 0xb0,
        // 类型管理
        CI_MAX = 0x80,
        CA_MAX = 0xf0,
        TYPE_ERROR = 0xff
    };

    explicit Interface(Type type, const QString &name, QObject *parent = 0);
    virtual ~Interface();

    Type type;              // 通信接口/应用类型
    QString name;           // 通信接口/应用名称
    bool connectedFlag;     // 通信接口连接状态，通信应用无效

    // 数据帧类型定义
    enum Frame
    {
        STREAM = 0x1010,
        CAN = 0x2000
    };

    // CAN 帧类型定义
    enum CanFrame
    {
        STANDARD = 0x2020,
        EXTENDED = 0x2030,
        RTR = 0x2040,       // remote transmission request time
        ERROR = 0x20f0
    };

    // Serial 波特率
    enum SerialBaudRate
    {
        SERIAL_BAUD_1200,
        SERIAL_BAUD_2400,
        SERIAL_BAUD_4800,
        SERIAL_BAUD_9600,
        SERIAL_BAUD_19200,
        SERIAL_BAUD_38400,
        SERIAL_BAUD_57600,
        SERIAL_BAUD_76800,
        SERIAL_BAUD_115200
    };

    // Serial 校验位
    enum SerialParity
    {
        SERIAL_PARITY_NONE, // 无校验
        SERIAL_PARITY_ODD,  // 奇校验
        SERIAL_PARITY_EVEN  // 偶校验
    };

    // data @ N+1
    static bool setInt(QByteArray &data, int i, int d, bool lsb = false);
    static void appendInt(QByteArray &data, int d);
    static bool setLong(QByteArray &data, int i, long d, bool lsb = false);
    static bool setLong(QByteArray &data, int i, char h0, char h1, char l0, char l1, bool lsb =false);
    static  int getInt(const QByteArray &data, int i, bool lsb = false);
    static long getLong(const QByteArray &data, int i, bool lsb = false);

    // time
    static bool checkFrame(const QByteArray &data, Frame type = STREAM);
    static void setStreamFrame(QByteArray &data);   // 设置为 stream 帧
    static void removeFrame(QByteArray &data);  // 去掉帧类型字符

    // can
        // check
    static bool checkCanFrame(const QByteArray &data, CanFrame type = EXTENDED);
        // set
    static bool setCanId(QByteArray &data, long id, CanFrame type = EXTENDED);
    static bool setCanData(QByteArray &data, char d0, char d1, char d2, char d3,\
                           char d4, char d5, char d6, char d7);
    static bool setCanData(QByteArray &data, int d0, int d1, int d2, int d3, bool lsb = false);
        // get
    static char getCanLength(const QByteArray &data);
    static char getCanDataChar(const QByteArray &data, int i = 0); // @ N+1
    static int  getCanDataInt(const QByteArray &data, int i = 0, bool lsb = false);
    static long getCanDataLong(const QByteArray &data, int i = 0, bool lsb = false);
    static uchar getCanDataBCD(const QByteArray &data, int i);
    static u_int8_t getBCD(u_int8_t d);

    // can j1939
        // check
    static bool checkJ1939PSSA(const QByteArray &data, char ps, char sa);
    static bool checkJ1939ID(const QByteArray &data, long id);
        // set
    static bool setJ1939Id(QByteArray &data, char p, char pf, char ps, char sa);
        // get
    static long getJ1939Id(const QByteArray &data);
    static long getJ1939PGN(const QByteArray &data);

    virtual bool initialize();

Q_SIGNALS:
    // 发送数据给通信路由
    void sendToRouting(QByteArray data);
    // 连接状态改变信号
    void connectChangedSignal(bool connect);

public Q_SLOTS:
    // 接收通信路由数据
    virtual void receiveFromRouting(QByteArray data);
    // 连接状态已改变，仅通信应用使用
    virtual void connectChanged(bool connect);

protected:
    bool initializedFlag;   // 是否已初始化:　1 已初始化  0 未初始化
    bool availableFlag;     // 是否可用，用于方法判断:　1 可用  0 不可用
    bool faultFlag;         // 是否接口故障或参数错误:　1 故障  0 正常
    QTimer *readTimer;  // 读取定时器
    Error *error;       // 错误记录变量
    // 设置连接状态
    void setConnect(bool connect);
};

#endif // INTERFACE_H
