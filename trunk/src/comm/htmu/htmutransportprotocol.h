#ifndef HTMUTRANSPORTPROTOCOL_H
#define HTMUTRANSPORTPROTOCOL_H

#include "interface.h"

// HTMU 传输协议
class HTMUTransportProtocol
{
public:
    explicit HTMUTransportProtocol(char pf, char ps, char sa);
    ~HTMUTransportProtocol();

    bool receiveFrame(const QByteArray &frame);
    const QByteArray &getData() const;

private:
    long canID;             // 报文 CAN ID
    QByteArray data;        // 报文
    int currentFrameNumber; // 当前报文序号
    int totalFrameNumber;   // 报文总数
    int currentDataLength;  // 当前数据长度
    int totalDataLength;    // 有效数据长度
};

#endif // HTMUTRANSPORTPROTOCOL_H
