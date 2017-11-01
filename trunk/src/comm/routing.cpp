#include "routing.h"

// 消息
#define registerMessage(s)          error ->message(QString("type[%1H] port[%2] name[%3]: ").\
                                    arg(port ->interface ->type, 2, 16).\
                                    arg(portNumber).\
                                    arg(port ->interface ->name)+\
                                    QString(s))
#define ruleMessage(s)              error ->message(QString("rule CI[%1] to CA[%2]: ").\
                                    arg(portNumberCI).\
                                    arg(portNumberCA)+\
                                    QString(s))
#define dataMessage(s)              error ->message(QString("[%1:%2H] port[%3] byte[%4B]: ").\
                                    arg(cri ->interface ->name).\
                                    arg(cri ->interface ->type, 2, 16).\
                                    arg(cri ->currentInterfaceNumber).\
                                    arg(data.size())+QString(s))

Routing *Routing::instance_ = 0;
Routing::Routing(int portNumber, QObject *parent) : QObject(parent)
{
    // 初始化
    isAvailable = false;
    // 检查参数
    if(portNumber > ROUTING_PORT_MAX * 5) return;
    this ->portNumber = portNumber;
    // 接口对象初始化
    for(int i = 0; i <= portNumber; i ++)
    {
        portCI.append(new RoutingPort(i));
        portCA.append(new RoutingPort(i));
        portCI.at(i) ->isRegistered = false;
        portCI.at(i) ->rule.isEnable = false;
        portCA.at(i) ->isRegistered = false;
        portCA.at(i) ->rule.isEnable = false;
    }
    // 错误处理类初始化
    error = new Error("ROUTING");
    isAvailable = true;
}

/*----------------------------------------------------------------------------
 * Function:        Routing::registerCR
 * Description:     CI/CA 注册和注销
 * param[1]:
 *--------------------------------------------------------------------------*/
void Routing::registerCR(uchar portNumber, const Interface &interface, bool isRegister)
{
    RoutingPort *port;
    error ->handleBase(ERROR_REGSITER);
    // 不可用
    if(!isAvailable)
    {
        error ->handle(-1);
        return;
    }
    // 接口编号超出范围
    if(portNumber > this ->portNumber)
    {
        error ->handle(-2);
        return;
    }
    // 检查类型，取得拟注册接口指针
    if(interface.type < Interface::CI_MAX)
    {
        port = portCI.at(portNumber);
    }
    else if(interface.type < Interface::CA_MAX)
    {
        port = portCA.at(portNumber);
    }
    else
    {
        error ->handle(-3);
        return;
    }
    // 接口已注册
    if(port ->isRegistered)
    {
        error ->handle(-4);
        registerMessage("port already registered [x]");
    }
    // 注册
    if(isRegister)
    {
        // 保存 interface
        port ->interface = &interface;
        // CRI<-->CI
        QObject::connect(port, SIGNAL(sendToExternal(QByteArray)), port ->interface, SLOT(receiveFromRouting(QByteArray)));
        QObject::connect(port ->interface, SIGNAL(sendToRouting(QByteArray)), port, SLOT(receiveFromExternal(QByteArray)));
        // CRI<-->CR
        QObject::connect(port, SIGNAL(sendToInternal(RoutingPort*,QByteArray&)), this, SLOT(receive(RoutingPort*,QByteArray&)));
        // 设置标志
        port ->rule.isEnable = false;
        port ->isRegistered = true;
        // 调试
        registerMessage("register ok [o]");
    }
    else
    {
        // 注销
        port ->isRegistered = false;
        port ->rule.isEnable = false;
        QObject::disconnect(port, 0, port ->interface, 0);
        QObject::disconnect(port ->interface, 0, port, 0);
        QObject::disconnect(port, 0, this, 0);
        // 调试
        registerMessage("unregistered ok [o]");
    }
}

/*----------------------------------------------------------------------------
 * Function:        setRoutingRule
 * Description:     设置路由规则
 * param[1]:        number：序号
 * param[2]:        param：路由规则
 * return:          void
 *--------------------------------------------------------------------------*/
void Routing::setRuleCR(int portNumberCI, int portNumberCA, bool isEnable)
{
    error ->handleBase(ERROR_SET);
    // 不可用
    if(!isAvailable)
    {
        error ->handle(-1);
        return;
    }
    // 接口编号超出范围
    if((portNumberCI > ROUTING_PORT_MAX)
    || (portNumberCA > ROUTING_PORT_MAX))
    {
        error ->handle(-2);
        return;
    }
    // 检查接口是否注册
    if((portCI.at(portNumberCI) ->isRegistered == false)
    || (portCA.at(portNumberCA) ->isRegistered == false))
    {
        error ->handle(-3); // 未注册
        ruleMessage("port unregistered [x]");
        return;
    }
    // 规则启用
    if(isEnable)
    {
        if((portCI.at(portNumberCI) ->rule.isEnable == false)
        && (portCA.at(portNumberCA) ->rule.isEnable == false))
        {
            portCI.at(portNumberCI) ->rule.destinationInterfaceNumber = portNumberCA;
            portCA.at(portNumberCA) ->rule.destinationInterfaceNumber = portNumberCI;
            QObject::connect(portCI.at(portNumberCI) ->interface, SIGNAL(connectChangedSignal(bool)),\
                             portCA.at(portNumberCA) ->interface, SLOT(connectChanged(bool)));
            portCI.at(portNumberCI) ->rule.isEnable = true;
            portCA.at(portNumberCA) ->rule.isEnable = true;
            ruleMessage("set ok [o]");
        }
        else
        {
            error ->handle(-4); // 已注册
            ruleMessage("already enabled [x]");
        }
    }
    // 规则停用
    else
    {
        if((portCI.at(portNumberCI) ->rule.isEnable == true)\
        && (portCA.at(portNumberCA) ->rule.isEnable == true))
        {
            if((portCI.at(portNumberCI) ->currentInterfaceNumber\
            == portCA.at(portNumberCA) ->rule.destinationInterfaceNumber)
            && (portCI.at(portNumberCA) ->currentInterfaceNumber\
            == portCA.at(portNumberCI) ->rule.destinationInterfaceNumber))
            {
                QObject::disconnect(portCI.at(portNumberCI) ->interface, 0,\
                                    portCA.at(portNumberCA) ->interface, 0);
                portCI.at(portNumberCI) ->rule.isEnable = false;
                portCA.at(portNumberCA) ->rule.isEnable = false;
                ruleMessage("disable ok [o]");
            }
            else
            {
                error ->handle(-5); // 不匹配
                ruleMessage("does not match [x]");
            }
        }
        else
        {
            error ->handle(-6); // 已禁用
            ruleMessage("already disabled [x]");
        }
    }
}

/*----------------------------------------------------------------------------
 * Function:        receive
 * Description:     接收数据流，根据路由规则转发给
 * param[1]:        cri: 路由接口指针
 * param[2]:        data：CR 数据流
 * return:          void
 *--------------------------------------------------------------------------*/
void Routing::receive(RoutingPort *cri, QByteArray &data)
{
    error ->handleBase(ERROR_RECEIVE);
    // 不可用
    if(!isAvailable)
    {
        error ->handle(-1);
        return;
    }
    // 指针为空
    if(cri == NULL)
    {
        error ->handle(-2);
    }
    // 接口未注册
    if(!cri ->isRegistered)
    {
        // 调试
        error ->handle(-3);
        return;
    }
    // 接口未连接
    if(!cri ->interface ->connectedFlag)
    {
        // 调试
        dataMessage("receive port no connected [x]");
        return;
    }
    // 无数据
    if(data.size() == 0)
    {
        // 调试
        dataMessage("no data [x]");
        return;
    }
    // 路由已启用
    if(cri ->rule.isEnable)
    {
        // 调试
        // disable dataMessage("data forwarding to ... [o]");
        if(cri ->interface -> type < Interface::CI_MAX)
        {
            send(portCA.at(cri ->rule.destinationInterfaceNumber), data);
        }
        else if(cri ->interface -> type < Interface::CA_MAX)
        {
            send(portCI.at(cri ->rule.destinationInterfaceNumber), data);
        }
    }
    else
    {
        dataMessage("rule disable [x]");
    }
}

/*----------------------------------------------------------------------------
 * Function:        send
 * Description:     发送数据
 * param[1]:        cri: 路由接口指针
 * param[2]:        data：CR 数据流
 * return:          void
 *--------------------------------------------------------------------------*/
void Routing::send(RoutingPort *cri, QByteArray &data)
{
    error ->handleBase(ERROR_SEND);
    // 不可用
    if(isAvailable == false)
    {
        error ->handle(-1);
        return;
    }
    // 指针为空
    if(cri == NULL)
    {
        error ->handle(-2);
        return;
    }
    // 接口未注册
    if(!cri ->isRegistered)
    {
        // 调试
        error ->handle(-3);
        return;
    }
    // 接口未连接
    if(!cri ->interface ->connectedFlag)
    {
        // 调试
        dataMessage("send port no connected [x]");
        return;
    }
    // 发送
    cri ->emitSendToExternalSignal(data);
    // 调试
    // disable dataMessage("send ok [o]");
}

/*----------------------------------------------------------------------------
 * Function:        RoutingPort
 * Description:     RoutingPort 构造函数
 *--------------------------------------------------------------------------*/
RoutingPort::RoutingPort(uchar portNumber) : QObject()
{
    currentInterfaceNumber = portNumber;
    receiveData.reserve(256);
}

void RoutingPort::emitSendToExternalSignal(QByteArray &data)
{
    emit sendToExternal(data);
}

void RoutingPort::receiveFromExternal(QByteArray data)
{
    receiveData = data;
    emit sendToInternal(this, receiveData);
}
