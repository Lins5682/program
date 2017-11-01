#ifndef ROUTING_H
#define ROUTING_H

#include <QObject>
#include <QList>
#include "interface.h"
// code QByteArray 预留字节
/*
 * ROUTING_PORT_MAX 定义
 *      接口最大数量 @ N+1
 *      定义了 CR 分别与 CI/CA 的信号槽数量
 * ROUTING_BUFFER_RESERVE_MAX 定义
 */
#define ROUTING_PORT_MAX                5

// 统计变量定义 code
struct DataStatistics
{
    int ch0;
    long ch1;
};

class RoutingPort;

// CR 类
class Routing : public QObject
{
    Q_OBJECT
public:
    explicit Routing(int portNumber = ROUTING_PORT_MAX, QObject *parent = 0);

    static Routing *instance()
    {
        if(!instance_)
        {
            instance_ = new Routing();
        }
        return instance_;
    }

    // 注册/注销
    void registerCR(uchar port, const Interface &interface, bool isRegister = true);
    // 设置路由规则
    void setRuleCR(int interfaceCI, int interfaceCA, bool isEnable = true);

    // code void connectTCPServer(int cich, long ip, int port);

public:
    static Routing *instance_;
private:
    // 可用标志
    bool isAvailable;
    // 端口数量
    int portNumber;
    // 错误记录变量
    Error *error;
    // 接口变量
    QList<RoutingPort *> portCI;
    QList<RoutingPort *> portCA;

private slots:
    // 接收和发送函数
    void receive(RoutingPort *cri, QByteArray &data);
    void send(RoutingPort *cri, QByteArray &data);
};

// CR 端口类
class RoutingPort : public QObject
{
    Q_OBJECT
public:
    explicit RoutingPort(uchar interfaceNumber);
    // 已注册标志
    bool isRegistered;    
    // CR 对象
    const Interface *interface;
    // 当前接口编号
    uchar currentInterfaceNumber;
    // 路由规则
    struct
    {
        bool isEnable;
        // 目标接口编号
        uchar destinationInterfaceNumber;
    }rule;

    // CR-Tx step0: CR-->CRI 执行发送数据给外部
    void emitSendToExternalSignal(QByteArray &data);

Q_SIGNALS:
    // CR-Tx step1: CRI-->CRO 发送数据给外部
    void sendToExternal(QByteArray data);
    // CR-Rx step1: CRI-->CR 发送数据给内部
    //code void sendToInternal(Interface::Type type, uchar interfaceNumber, const QByteArray &data);
    void sendToInternal(RoutingPort *cri, QByteArray &data);

public Q_SLOTS:
    // CR-Rx step0: CRO-->CRI 收到外部数据
    void receiveFromExternal(QByteArray data);

private:
    QByteArray receiveData;
};

#endif // ROUTING_H
