#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include "can/interfacecan.h"

InterfaceCan::InterfaceCan(const QString &name) :
    Interface(CI_CAN, name)
{
    interfaceName = name;
}

InterfaceCan::~InterfaceCan()
{
}

bool InterfaceCan::initialize()
{
    if(!Interface::initialize()) return false;
    error ->messageType(Error::INITIALIZE_START);
    // 设备名称错误
    if((interfaceName != "can0") && (interfaceName != "can1"))
    {
        error ->handle(-3);
        return false;
    }
    // 创建 Socket CAN 套接字
    socketFD = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(socketFD < 0)
    {
        error ->handle(-4);
        return false;
    }
    error ->message(interfaceName + QString(" socketFD = %1 [i]").arg(socketFD));
    // 复制名称
    ifreq interfaceREQ;
    QByteArray b = interfaceName.toLatin1();
    const char *c = b.data();
    strcpy((char *)(interfaceREQ.ifr_name), c);
    // 指定 CAN 设备
    if(ioctl(socketFD, SIOCGIFINDEX, &interfaceREQ) < 0)
    {
        error ->handle(-5);
        return false;
    }
    error ->message(interfaceName + QString(" ifindex = %1 [i]").arg(interfaceREQ.ifr_ifindex));
    // 协议族
    sockaddr_can socketAddress;
    socketAddress.can_family = AF_CAN;
    socketAddress.can_ifindex = interfaceREQ.ifr_ifindex;
    // 将套接字与 CAN 绑定
    if(bind(socketFD, (sockaddr *)&socketAddress, sizeof(socketAddress)) < 0)
    {
        error ->handle(-6);
        return false;
    }
    // 设置 CAN 滤波：接收全部，不设置则接受全部
    /*
    can_filter filter;
    int loopback = 0;
    filter.can_id = 0;
    filter.can_mask = 0;
    setsockopt(canfd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    setsockopt(canfd, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
    */
    // 接收定时器
    readTimer ->setSingleShot(false);
    readTimer ->setInterval(2);
    QObject::connect(readTimer, SIGNAL(timeout()), this, SLOT(readDevice()));
    readTimer ->start();
    // 标志位
    initializedFlag = true;
    availableFlag = true;
    error ->messageType(Error::INITIALIZE_OK);
    return true;
}

void InterfaceCan::receiveFromRouting(QByteArray data)
{
    error ->handleBase(ERROR_RECEIVE);
    // 不可用
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    // 检查数据格式 CAN
    if(!checkFrame(data, CAN))
    {
        error ->handle(-2);
        return;
    }
    can_frame buf;
    buf.can_id = getLong(data, 0);
    buf.can_dlc = data.at(4);
    for(int i = 0, j = 5; i <= 7; i ++, j++) buf.data[i] = data.at(j);
    int n = send(socketFD, &buf, sizeof(buf), 0);
    if(n == -1)
    {
        error ->handle(-3);
        return;
    }
    else if(n != sizeof(buf))
    {
        error ->handle(-4);
        return;
    }
}

void InterfaceCan::readDevice()
{
    error ->handleBase(ERROR_READ);
    // 不可用
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    //接收报文
    can_frame buf;
    int n = recv(socketFD, &buf, sizeof(buf), MSG_DONTWAIT);
    if(n == sizeof(buf))
    {
        QByteArray data(15, 0);
        setLong(data, 0, buf.can_id);
        data[4] = buf.can_dlc;
        for(int i = 0, j = 5; i <= 7; i ++, j ++) data[j] = buf.data[i];
        // 检查帧类型
        if((data.at(0) & 0b01100000) == 0)
        {
            // 是扩展帧
            if(data.at(0) & 0b10000000) setInt(data, 13, EXTENDED);
            // 是标准帧
            else setInt(data, 13, STANDARD);
        }
        // 是错误帧
        else if(data.at(0) & 0b00100000)
        {
            setInt(data, 13, ERROR);
        }
        // 是远程帧
        else if(data.at(0) & 0b01000000)
        {
            setInt(data, 13, RTR);
        }
        // 去掉标志位
        data[0] = data.at(0) & 0b00011111;
        // 转发给路由
        Q_EMIT sendToRouting(data);
    }
    else if(n > 0)
    {
        error ->handle(-2);
    }
}
