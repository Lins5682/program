#ifndef HTMU_H
#define HTMU_H

#include "htmuportdc.h"
#include "htmutransportprotocol.h"

class HTMU : public Interface
{
    Q_OBJECT
public:
    explicit HTMU(const QString &name, char number, char id = 0);
    ~HTMU();

    // 设置 HTCU 版本
    static void setHtcuVersion(uint v);
    // 设置参数
    void setParameter(long n, char rc0, char rc1, char rc2);
    // 获取 Port 类
    const HTMUPortDC &portData();
    // 初始化
    virtual bool initialize();

Q_SIGNALS:
    // Port 数据更新
    void portDataUpdated(int pgn);
    // 启动/停止充电状态
    void chargingState(int s); // 见 HTMUPort::State
    
public Q_SLOTS:
    // 启动充电
    void requestStartCharging(uchar lcw);
    // 停止充电
    void requestStopCharging(uchar sr);
    // 设置状态
    void setState(uchar s, uchar kwh, uchar time);

    // HTMU slot
    void refresh(void);
    virtual void receiveFromRouting(QByteArray data);

private:
    char number;                // HTMU 编号
    char id;                    // HTMU 充电端口标识
    static uchar htcuAddress;   // HTCU CAN 地址
    static uint htcuVersion;    // HTCU 通信协议版本
    static uchar htcuState;     // HTCU 状态：0-正常 1-故障

    // 状态
    enum State
    {
        POST_NULL,
        POST_START,
        POST_CHECK_VERSION,
        POST_SET_PARAMETER,
        REQUEST_STANDBY,
        STANDBY,
        REQUEST_START_CHARGING,
        START_CHARGING,
        START_COMPLETE,
        REQUEST_CHARGING,
        CHARGING,
        REQUEST_STOP_CHARGING,
        STOP_CHARGING,
        STOP_COMPLETE
    };
    State state;
    State request_state;
    long postCount;

    HTMUPortDC *port;
    HTMUTransportProtocol *pgn11Tp;
    HTMUTransportProtocol *pgn13Tp;
    HTMUTransportProtocol *pgn22Tp;

    void setFlag(char c, char mask, bool *b);
    void setHtmuCanId(QByteArray &data, char p, char pf);

    void sendTime();    // 05H
    bool sendVersion(bool r = false);   // 07H*
    bool sendParameter(bool r = false); // 09H*
    bool sendStartCharging(bool r = false); // 01H*
    bool sendStopCharging(bool r =false);   // 03H*
    bool sendStartChargingCompleteResponse(bool r = false, bool s = false); // 12H*
    bool sendStopChargingCompleteResponse(bool r = false, bool s = false);  // 14H*
    bool sendConnectConfirmResponse(bool s = false); // 16H*
    void sendLife();    // 31H*
};

#endif // HTMU_H
