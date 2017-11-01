#include "htmu.h"
#include <QDebug>

#define REFRESH_PERIOD      20

uchar HTMU::htcuAddress = 0;
uint  HTMU::htcuVersion = 0;
uchar HTMU::htcuState = 0;

HTMU::HTMU(const QString &name, char number, char id) :
    Interface(CA_QYHT_MONITORING_UNIT, name)
{
    this ->number = number;
    this ->id = id;
    port = NULL;
    pgn11Tp = NULL;
    pgn13Tp = NULL;
    pgn22Tp = NULL;

    request_state = POST_NULL;
}

HTMU::~HTMU()
{
    if(port != NULL) delete port;
    if(pgn11Tp != NULL) delete pgn11Tp;
    if(pgn13Tp != NULL) delete pgn13Tp;
    if(pgn22Tp != NULL) delete pgn22Tp;
}

void HTMU::setHtcuVersion(uint v)
{
    htcuVersion = v;
}

void HTMU::setParameter(long n, char rc0, char rc1, char rc2)
{
    port ->pgn09SetParameter.ccmNumber = n;
    port ->pgn09SetParameter.regionalCode[0] = rc0;
    port ->pgn09SetParameter.regionalCode[1] = rc1;
    port ->pgn09SetParameter.regionalCode[2] = rc2;
}

const HTMUPortDC &HTMU::portData()
{
    return *port;
}

/*----------------------------------------------------------------------------
 * Function:        VIRTUAL initialize
 * Description:     初始化
 *--------------------------------------------------------------------------*/
bool HTMU::initialize()
{
    if(!Interface::initialize()) return false;
    error ->messageType(Error::INITIALIZE_START);
    // HTCU 地址
    htcuAddress = 0x8a;
    // 直流充电端口
    if(number > 7)
    {
        error ->handle(-3);
        return false;
    }
    port = new HTMUPortDC(number, id);

    port ->pgn15ConnectConfrm.receiveFlag = false;

    // 传输协议
    pgn11Tp = new HTMUTransportProtocol(0x11, htcuAddress, port ->address);
    pgn13Tp = new HTMUTransportProtocol(0x13, htcuAddress, port ->address);
    pgn22Tp = new HTMUTransportProtocol(0x22, htcuAddress, port ->address);
    // 定时器
    readTimer ->setSingleShot(false);
    readTimer ->setInterval(REFRESH_PERIOD);
    QObject::connect(readTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    readTimer ->start();
    // 标志位
    initializedFlag = true;
    availableFlag = true;
    state = POST_START;
    // 消息
    error ->messageType(Error::INITIALIZE_OK);
    return true;
}

/*----------------------------------------------------------------------------
 * Function:        SLOT requestStartCharging
 * Description:     请求启动充电
 *--------------------------------------------------------------------------*/
void HTMU::requestStartCharging(uchar lcw)
{
    port ->pgn01StartCharging.loadControlSwitch = lcw;
    request_state = REQUEST_START_CHARGING;
}

/*----------------------------------------------------------------------------
 * Function:        SLOT requestStopCharging
 * Description:     请求停止充电
 *--------------------------------------------------------------------------*/
void HTMU::requestStopCharging(uchar sr)
{
    port ->pgn03StopCharging.stopReason = sr;
    request_state = REQUEST_STOP_CHARGING;
}

/*----------------------------------------------------------------------------
 * Function:        SLOT setState
 * Description:     设置状态
 *--------------------------------------------------------------------------*/
void HTMU::setState(uchar s, uchar kwh, uchar time)
{
    htcuState = s;
    port ->pgn31Life.chargeKWH = kwh;
    port ->pgn31Life.chargeTime = time;
}

/*----------------------------------------------------------------------------
 * Function:        SLOT refresh
 * Description:     固定周期刷新
 *--------------------------------------------------------------------------*/
void HTMU::refresh()
{
    // 连接确认
    // disable
    if(port ->pgn15ConnectConfrm.receiveFlag == true)
    {
        port ->pgn15ConnectConfrm.receiveFlag = false;
        port ->pgn15ConnectConfrm.okFlag = true;
        sendConnectConfirmResponse(true);
        request_state = REQUEST_START_CHARGING;
    }
//    else if(sendConnectConfirmResponse() == true)
//    {
//        port ->pgn15ConnectConfrm.okFlag = false;
//    }
//    port ->pgn15ConnectConfrm.okFlag = true;

    switch(state)
    {
    case REQUEST_STANDBY:
    {
        state = STANDBY;
        error ->message(QString("standby [i]"));
        break;
    }
    case STANDBY:
    {
        if(request_state == REQUEST_START_CHARGING)
        {
            if(port ->pgn15ConnectConfrm.okFlag == true) // 已连接，发送启动充电报文
            {
                state = REQUEST_START_CHARGING;
            }
            else // 未连接，发射状态信号
            {
                port ->chargingState = HTMUPortDC::GUN_NOT_CONNECTED;
                Q_EMIT chargingState(port ->chargingState);
                error ->message(QString("request failure, charging gun not connected [x]"));
            }
        }
        break;
    }
    case REQUEST_CHARGING:
    {
        state = CHARGING;
        error ->message(QString("charging [i]"));
        break;
    }
    case CHARGING:
    {
        if(request_state == REQUEST_STOP_CHARGING) state = REQUEST_STOP_CHARGING;
        break;
    }
    case REQUEST_START_CHARGING:
    {
        if(request_state == REQUEST_STOP_CHARGING)
        {
            state = REQUEST_STOP_CHARGING;
            break;
        }
        sendStartCharging(true);
        port ->pgn02StartChargingResponse.okFlag = false;
        port ->pgn02StartChargingResponse.receiveFlag = false;
        state = START_CHARGING;
        error ->message(QString("request start charging [i]"));
        break;
    }
    case START_CHARGING:
    {
        if(request_state == REQUEST_STOP_CHARGING)
        {
            state = REQUEST_STOP_CHARGING;
            break;
        }
        if(port ->pgn02StartChargingResponse.receiveFlag == true)
        {
            port ->pgn02StartChargingResponse.receiveFlag = false;
            if(port ->pgn02StartChargingResponse.confirmID == 0) // 成功
            {
                port ->pgn02StartChargingResponse.okFlag = true;
                port ->chargingState = HTMUPortDC::START_CHARGING_SUCCESS;
                Q_EMIT chargingState(port ->chargingState);
                    // next
                state = START_COMPLETE;
                sendStartChargingCompleteResponse(true);
                port ->pgn11StartChargingCompleteResponse.okFlag = false;
                port ->pgn11StartChargingCompleteResponse.receiveFlag = false;
                error ->message(QString("receive start charging ok [o]"));
                break;
            }
        }
        if(sendStartCharging() == true)
        {
            port ->pgn02StartChargingResponse.okFlag = false;
            port ->chargingState = HTMUPortDC::START_CHARGING_FAILURE;
            Q_EMIT chargingState(port ->chargingState);
                // next
            state = REQUEST_STANDBY;
            error ->message(QString("receive start charging timeout [x]"));
        }
        break;
    }
    case START_COMPLETE:
    {
        if(request_state == REQUEST_STOP_CHARGING)
        {
            state = REQUEST_STOP_CHARGING;
            break;
        }
        // code
        if(port ->pgn11StartChargingCompleteResponse.receiveFlag == true)
        {
            port ->pgn11StartChargingCompleteResponse.receiveFlag = false;
            if(port ->pgn11StartChargingCompleteResponse.confirmID == 0) // 成功
            {
                port ->pgn11StartChargingCompleteResponse.okFlag = true;
                port ->chargingState = HTMUPortDC::START_CHARGING_COMPLETE_SUCCESS;
                Q_EMIT chargingState(port ->chargingState);
                sendStartChargingCompleteResponse(false, true);
                    // next
                state = REQUEST_CHARGING;
                error ->message(QString("receive start charging complete ok [o]"));
                break;
            }
        }
        if(sendStartChargingCompleteResponse() == true)
        {
            port ->pgn11StartChargingCompleteResponse.okFlag = false;
            port ->chargingState = HTMUPortDC::START_CHARGING_COMPLETE_FAILURE;
            Q_EMIT chargingState(port ->chargingState);
                // next
            state = REQUEST_STOP_CHARGING;
            error ->message(QString("receive start charging complete timeout [x]"));
        }
        break;
    }
    case REQUEST_STOP_CHARGING:
    {
        sendStopCharging(true);
        port ->pgn04StopChargingResponse.okFlag = false;
        port ->pgn04StopChargingResponse.receiveFlag = false;
        state = STOP_CHARGING;
        error ->message(QString("request stop charging [i]"));
        break;
    }
    case STOP_CHARGING:
    {
        if(port ->pgn04StopChargingResponse.receiveFlag == true)
        {
            port ->pgn04StopChargingResponse.receiveFlag = false;
            if(port ->pgn04StopChargingResponse.confirmID == 0) // 成功
            {
                port ->pgn04StopChargingResponse.okFlag = true;
                port ->chargingState = HTMUPortDC::STOP_CHARGING_SUCCESS;
                Q_EMIT chargingState(port ->chargingState);
                    // next
                state = STOP_COMPLETE;
                sendStopChargingCompleteResponse(true);
                port ->pgn13StopChargingCompleteResponse.okFlag = false;
                port ->pgn13StopChargingCompleteResponse.receiveFlag = false;
                error ->message(QString("receive stop charging ok [o]"));
                break;
            }
        }
        if(sendStopCharging() == true)
        {
            port ->pgn04StopChargingResponse.okFlag = false;
            port ->chargingState = HTMUPortDC::STOP_CHARGING_FAILURE;
            Q_EMIT chargingState(port ->chargingState);
                // next
            state = REQUEST_STANDBY;
            error ->message(QString("receive stop charging timeout [x]"));
        }
        break;
    }
    case STOP_COMPLETE:
    {
        if(port ->pgn13StopChargingCompleteResponse.receiveFlag == true)
        {
            if(port ->pgn13StopChargingCompleteResponse.confirmID == 0) // 成功
            {
                port ->pgn13StopChargingCompleteResponse.okFlag = true;
                port ->chargingState = HTMUPortDC::STOP_CHARGING_COMPLETE_SUCCESS;
                Q_EMIT chargingState(port ->chargingState);
                sendStopChargingCompleteResponse(false, true);
                    // next
                state = STANDBY;
                error ->message(QString("receive stop charging complete ok [o]"));
                break;
            }
        }
        if(sendStopChargingCompleteResponse() == true)
        {
            port ->chargingState = HTMUPortDC::STOP_CHARGING_COMPLETE_FAILURE;
            Q_EMIT chargingState(port ->chargingState);
                // next
            state = REQUEST_STANDBY;
            error ->message(QString("receive stop charging complete timeout [x]"));
        }
        break;
    }
    case POST_SET_PARAMETER:
    {
        if(port ->pgn0aSetParameterResponse.receiveFlag == true)
        {
            port ->pgn0aSetParameterResponse.receiveFlag = false;
            //disable if(port ->pgn0aSetParameterResponse.confirmID == 0) // 成功
            //if(port ->pgn0aSetParameterResponse.confirmID == 0x0b) // 成功
            {
                port ->pgn0aSetParameterResponse.okFlag = true;
                    // next
                state = REQUEST_STANDBY;
                error ->message(QString("set parameter ok [o]"));
                break;
            }
        }
        if(sendParameter() == true)
        {
            port ->pgn0aSetParameterResponse.okFlag = false;
                // next
            state = POST_START;
            error ->message(QString("set parameter timeout [x]"));
        }
        break;
    }
    case POST_CHECK_VERSION:
    {
        if(port ->pgn08CheckVersionResponse.receiveFlag == true)
        {
            port ->pgn08CheckVersionResponse.receiveFlag = false;
            if(port ->pgn08CheckVersionResponse.version == htcuVersion) // 匹配
            {
                port ->pgn08CheckVersionResponse.okFlag = true;
                    // next
                state = POST_SET_PARAMETER;
                sendParameter(true);
                port ->pgn0aSetParameterResponse.okFlag = false;
                port ->pgn0aSetParameterResponse.receiveFlag = false;
                error ->message(QString("check version ok [o]"));
                break;
            }
        }
        if(sendVersion() == true)
        {
            port ->pgn08CheckVersionResponse.okFlag = false;
                // next
            state = POST_START;
            error ->message(QString("check version timeout [x]"));
        }
        break;
    }
    case POST_START:
    {
        state = POST_CHECK_VERSION;
        sendVersion(true);
        port ->pgn08CheckVersionResponse.okFlag = false;
        port ->pgn08CheckVersionResponse.receiveFlag = false;
            // count
        postCount ++;
        error ->message(QString("post start [i]"));
        break;
    }
    case POST_NULL:
    {
        state = POST_START;
        break;
    }
    }

    // 固定帧
//    sendLife();

    // 清除请求变量
    request_state = POST_NULL;
}

/*----------------------------------------------------------------------------
 * Function:        VIRTUAL SLOT receiveFromRouting
 * Description:     接收路由
 *--------------------------------------------------------------------------*/
void HTMU::receiveFromRouting(QByteArray data)
{    
    error ->handleBase(ERROR_RECEIVE);
    // 不可用
    if(!availableFlag)
    {
        error ->handle(-1);
        return;
    }
    // 检查数据格式 CAN FRAME EXTENDED
    if(!checkCanFrame(data))
    {
        error ->handle(-2);
        return;
    }

    // 检查 PS SA
    if(!checkJ1939PSSA(data, htcuAddress, port ->address)) return;

    // 检查 LEN
    // disable if(getCanLength(data) != 8) return;

    // PGN
    switch(getJ1939PGN(data))
    {
    case 0x000200: // 启动充电应答*
    {
        if(getCanDataChar(data, 0) != port ->id) break;
        port ->pgn02StartChargingResponse.loadControlSwitch = getCanDataChar(data, 1);
        port ->pgn02StartChargingResponse.confirmID = getCanDataChar(data, 2);
        port ->pgn02StartChargingResponse.receiveFlag = true;
        Q_EMIT portDataUpdated(0x0002);
        break;
    }
    case 0x000400: // 停止充电应答*
    {
        if(getCanDataChar(data, 0) != port ->id) break;
        port ->pgn04StopChargingResponse.confirmID = getCanDataChar(data, 1);
        port ->pgn04StopChargingResponse.receiveFlag = true;
        Q_EMIT portDataUpdated(0x0004);
        break;
    }
    case 0x000600: // 对时 code
    {
        port ->pgn06Time.immediateExecution = getCanDataChar(data, 0);
        port ->pgn06Time.confirmID = getCanDataChar(data, 1);
        Q_EMIT portDataUpdated(0x0006);
        break;
    }
    case 0x000800: // 通信协议版本*
    {
        port ->pgn08CheckVersionResponse.version = getCanDataBCD(data, 5);
        port ->pgn08CheckVersionResponse.version *= 100;
        port ->pgn08CheckVersionResponse.version += getCanDataBCD(data, 6);
        port ->pgn08CheckVersionResponse.receiveFlag = true;
        Q_EMIT portDataUpdated(0x0008);
        break;
    }
    case 0x000a00: // 充电桩参数设置应答*
    {
        port ->pgn0aSetParameterResponse.confirmID = getCanDataChar(data, 0);
        port ->pgn0aSetParameterResponse.receiveFlag = true;
        Q_EMIT portDataUpdated(0x000a);
        break;
    }
    case 0x001100: // 启动充电完成*
    {
        qDebug() << "--------0x11";
        if(pgn11Tp ->receiveFrame(data) != true) return;
        data = pgn11Tp ->getData();
        if(data.size() < 34) return;
        if(getCanDataChar(data, 0) != port ->id) break;
        port ->pgn11StartChargingCompleteResponse.loadControlSwitch = getCanDataChar(data, 1);
        port ->pgn11StartChargingCompleteResponse.confirmID = getCanDataChar(data, 2);
        port ->pgn11StartChargingCompleteResponse.ccm.protocolVersion0 = getCanDataChar(data, 3);
        port ->pgn11StartChargingCompleteResponse.ccm.protocolVersion1 = getCanDataChar(data, 4);
        port ->pgn11StartChargingCompleteResponse.ccm.protocolVersion2 = getCanDataChar(data, 5);
        port ->pgn11StartChargingCompleteResponse.bms.protocolVersion0 = getCanDataChar(data, 6);
        port ->pgn11StartChargingCompleteResponse.bms.protocolVersion1 = getCanDataChar(data, 7);
        port ->pgn11StartChargingCompleteResponse.bms.protocolVersion2 = getCanDataChar(data, 8);
        port ->pgn11StartChargingCompleteResponse.ccm.handshakeResult = getCanDataChar(data, 9);
        port ->pgn11StartChargingCompleteResponse.bms.batteryType = getCanDataChar(data, 10);
        port ->pgn11StartChargingCompleteResponse.bms.maximumAllowableTemperature = getCanDataChar(data, 11);
        port ->pgn11StartChargingCompleteResponse.bms.maximumAllowableChargingVoltage = getCanDataInt(data, 12, true);
        port ->pgn11StartChargingCompleteResponse.bms.maximumAllowableSingleVoltage = getCanDataInt(data, 14, true);
        port ->pgn11StartChargingCompleteResponse.bms.maximumAllowableChargingCurrent = getCanDataInt(data, 16, true);
        port ->pgn11StartChargingCompleteResponse.bms.ratedVoltage = getCanDataInt(data, 18, true);
        port ->pgn11StartChargingCompleteResponse.bms.currentVoltage = getCanDataInt(data, 20, true);
        port ->pgn11StartChargingCompleteResponse.bms.ratedCapacity = getCanDataInt(data, 22, true);
        port ->pgn11StartChargingCompleteResponse.bms.nominalEnergy = getCanDataInt(data, 24, true);
        port ->pgn11StartChargingCompleteResponse.bms.soc = getCanDataInt(data, 26, true);
        port ->pgn11StartChargingCompleteResponse.ccm.maximumOutputVoltage = getCanDataInt(data, 28, true);
        port ->pgn11StartChargingCompleteResponse.ccm.minimumOutputVoltage = getCanDataInt(data, 30, true);
        port ->pgn11StartChargingCompleteResponse.ccm.maximumOutputCurrent = getCanDataInt(data, 32, true);
        port ->pgn11StartChargingCompleteResponse.receiveFlag = true;
        Q_EMIT portDataUpdated(0x0011);
        break;
    }
    case 0x001300: // 停止充电完成*
    {
        if(pgn11Tp ->receiveFrame(data) != true) return;
        data = pgn11Tp ->getData();
        if(data.size() < 32) return;
        if(getCanDataChar(data, 0) != port ->id) break;
        port ->pgn13StopChargingCompleteResponse.stopReason = getCanDataChar(data, 1);
        port ->pgn13StopChargingCompleteResponse.confirmID = getCanDataChar(data, 2);
        port ->pgn13StopChargingCompleteResponse.bms.stopChargingReason = getCanDataChar(data, 3);
        port ->pgn13StopChargingCompleteResponse.bms.chargingFaultReason0 = getCanDataChar(data, 4);
        port ->pgn13StopChargingCompleteResponse.bms.chargingFaultReason1 = getCanDataChar(data, 5);
        port ->pgn13StopChargingCompleteResponse.bms.stopErrorReason = getCanDataChar(data, 6);
        port ->pgn13StopChargingCompleteResponse.bms.stopSoc = getCanDataChar(data, 7);
        port ->pgn13StopChargingCompleteResponse.bms.minimumSingleVoltage = getCanDataInt(data, 8, true);
        port ->pgn13StopChargingCompleteResponse.bms.maximumSingleVoltage = getCanDataInt(data, 10, true);
        port ->pgn13StopChargingCompleteResponse.bms.minimumTemperature = getCanDataChar(data, 12);
        port ->pgn13StopChargingCompleteResponse.bms.maximumTemperature = getCanDataChar(data, 13);
        port ->pgn13StopChargingCompleteResponse.bms.errorCode0 = getCanDataChar(data, 14);
        port ->pgn13StopChargingCompleteResponse.bms.errorCode1 = getCanDataChar(data, 15);
        port ->pgn13StopChargingCompleteResponse.bms.errorCode2 = getCanDataChar(data, 16);
        port ->pgn13StopChargingCompleteResponse.bms.errorCode3 = getCanDataChar(data, 17);
        port ->pgn13StopChargingCompleteResponse.ccm.errorCode0 = getCanDataChar(data, 18);
        port ->pgn13StopChargingCompleteResponse.ccm.errorCode1 = getCanDataChar(data, 19);
        port ->pgn13StopChargingCompleteResponse.ccm.errorCode2 = getCanDataChar(data, 20);
        port ->pgn13StopChargingCompleteResponse.ccm.errorCode3 = getCanDataChar(data, 21);
        port ->pgn13StopChargingCompleteResponse.receiveFlag = false;
        Q_EMIT portDataUpdated(0x0013);
        break;
    }
    case 0x001500: // 连接确认*
    {
        if(Interface::getCanDataChar(data, 0) != port ->id) break;
        port ->pgn15ConnectConfrm.receiveFlag = true;
        Q_EMIT portDataUpdated(0x0015);
        break;
    }
    case 0x002100: // 遥信*
    {
        if(Interface::getCanDataChar(data, 0) != port ->id) break;
        uchar c;
        // data1
        c = getCanDataChar(data, 1);
        port ->pgn21RemoteDi.ccm.workingState = c & 0x0f;
        setFlag(c, 1 << 4, &port ->pgn21RemoteDi.bms.communicationFault);
        setFlag(c, 1 << 5, &port ->pgn21RemoteDi.ccm.output.overVoltage);
        setFlag(c, 1 << 6, &port ->pgn21RemoteDi.ccm.output.underVoltage);
        setFlag(c, 1 << 7, &port ->pgn21RemoteDi.bms.battery.overCurrent);
        // data2
        c = getCanDataChar(data, 2);
        setFlag(c, 1 << 0, &port ->pgn21RemoteDi.bms.battery.overTemperature);
        setFlag(c, 1 << 1, &port ->pgn21RemoteDi.ccm.battery.connectedState);
        setFlag(c, 1 << 2, &port ->pgn21RemoteDi.ccm.emergencyStopAction);
        setFlag(c, 1 << 3, &port ->pgn21RemoteDi.ccm.insulationFault);
        setFlag(c, 1 << 4, &port ->pgn21RemoteDi.ccm.battery.reverseFault);
        setFlag(c, 1 << 5, &port ->pgn21RemoteDi.ccm.arresterFault);
        setFlag(c, 1 << 6, &port ->pgn21RemoteDi.ccm.gun.notHoming);
        setFlag(c, 1 << 7, &port ->pgn21RemoteDi.ccm.gun.overTemperature);
        // data3
        c = getCanDataChar(data, 3);
        setFlag(c, 1 << 0, &port ->pgn21RemoteDi.ccm.smokeAlarm);
        setFlag(c, 1 << 1, &port ->pgn21RemoteDi.ccm.input.overVoltage);
        setFlag(c, 1 << 2, &port ->pgn21RemoteDi.ccm.input.underVoltage);
        setFlag(c, 1 << 3, &port ->pgn21RemoteDi.cpm.fault);
        setFlag(c, 1 << 4, &port ->pgn21RemoteDi.ccm.output.contactorState);
        setFlag(c, 1 << 5, &port ->pgn21RemoteDi.cpm.fanFault);
        setFlag(c, 1 << 6, &port ->pgn21RemoteDi.cpm.overTemperature);
        setFlag(c, 1 << 7, &port ->pgn21RemoteDi.cpm.input.alarm);
        // data4
        c = getCanDataChar(data, 4);
        setFlag(c, 1 << 0, &port ->pgn21RemoteDi.cpm.output.shortCircuit);
        setFlag(c, 1 << 1, &port ->pgn21RemoteDi.cpm.output.overCurrent);
        setFlag(c, 1 << 2, &port ->pgn21RemoteDi.cpm.output.overVoltage);
        setFlag(c, 1 << 3, &port ->pgn21RemoteDi.cpm.output.underVoltage);
        setFlag(c, 1 << 4, &port ->pgn21RemoteDi.cpm.input.overVoltage);
        setFlag(c, 1 << 5, &port ->pgn21RemoteDi.cpm.input.underVoltage);
        setFlag(c, 1 << 6, &port ->pgn21RemoteDi.cpm.input.phaseLoss);
        setFlag(c, 1 << 7, &port ->pgn21RemoteDi.cpm.communicationAlarm);
        // data5
        c = getCanDataChar(data, 5);
        setFlag(c, 1 << 0, &port ->pgn21RemoteDi.ccm.controlGuidanceAlarm);
        setFlag(c, 1 << 1, &port ->pgn21RemoteDi.ccm.input.circuitBreakerFault);
        setFlag(c, 1 << 2, &port ->pgn21RemoteDi.ccm.output.overCurrent);
        setFlag(c, 1 << 3, &port ->pgn21RemoteDi.ccm.output.fuseFault);
        // data67
        port ->pgn21RemoteDi.other.faultNumber = getCanDataChar(data, 6);
        port ->pgn21RemoteDi.other.faultCode = getCanDataChar(data, 7);
        Q_EMIT portDataUpdated(0x0021);
        break;
    }
    case 0x002200: // 遥测*
    {
        if(pgn22Tp ->receiveFrame(data) != true) return;
        data = pgn22Tp ->getData();
        if(data.size() < 19) return;
        if(getCanDataChar(data, 0) != port ->id) break;
        port ->pgn22RemoteAi.ccm.outputVoltage = getCanDataInt(data, 1, true);
        port ->pgn22RemoteAi.ccm.outputCurrent = getCanDataInt(data, 3, true);
        port ->pgn22RemoteAi.bms.soc = getCanDataInt(data, 5, true);
        port ->pgn22RemoteAi.bms.minimumTemperature = getCanDataInt(data, 7, true);
        port ->pgn22RemoteAi.bms.maximumTemperature = getCanDataInt(data, 9, true);
        port ->pgn22RemoteAi.bms.maximumSingleVoltage = getCanDataInt(data, 11, true);
        port ->pgn22RemoteAi.bms.minimumSingleVoltage = getCanDataInt(data, 13, true);
        port ->pgn22RemoteAi.ccm.temperature = getCanDataInt(data, 15, true);
        port ->pgn22RemoteAi.ccm.controlGuidanceVoltage = getCanDataInt(data, 17, true);
        Q_EMIT portDataUpdated(0x0022);
        break;
    }
    case 0x00f000: // 测试 code
    {
        requestStartCharging(2);
        break;
    }
    case 0x00f100: // 测试 code
    {
        requestStopCharging(3);
        break;
    }
    }
}

/*----------------------------------------------------------------------------
 * Function:        setFlag
 * Description:     设置标志
 *--------------------------------------------------------------------------*/
void HTMU::setFlag(char c, char mask, bool *b)
{
    if(c & mask) *b = false;
    else *b = true;
}

/*----------------------------------------------------------------------------
 * Function:        setHtmuCanId
 * Description:     设置 HTMU 的 CAN ID
 *--------------------------------------------------------------------------*/
void HTMU::setHtmuCanId(QByteArray &data, char p, char pf)
{
    if(!availableFlag) return;
    setJ1939Id(data, p, pf, port ->address, htcuAddress);
}

/*----------------------------------------------------------------------------
 * Function:        sendTime
 * Description:     发送时间帧 --- 05H
 *--------------------------------------------------------------------------*/
void HTMU::sendTime()
{
    /* code QByteArray frame;
    frame.resize(15);
    setHtmuCanId(frame, 6, 0x05);
    int S = s * 1000;
    setCanData(frame, c, y, M, d, h, m, s, s >> 8);
    Q_EMIT sendToRouting(frame);*/
}

/*----------------------------------------------------------------------------
 * Function:        sendVersion
 * Description:     发送版本帧 --- 07H
 *--------------------------------------------------------------------------*/
bool HTMU::sendVersion(bool r)
{
    static uint i, j;
    if(r == true)
    {
        i = 60000;
        j = 0;
        return false;
    }

    i += REFRESH_PERIOD;
    if(i >= 500)
    {
        i = 0;
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 6, 0x07);
        setCanData(frame,
                   getBCD(htcuVersion / 100), getBCD(htcuVersion % 100),
                   0, 0, 0, 0, 0, 0);
        Q_EMIT sendToRouting(frame);
    }

    j += REFRESH_PERIOD;
    if(j >= 5000)
    {
        j = 0;
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------------
 * Function:        sendParameter
 * Description:     发送参数帧 --- 09H
 *--------------------------------------------------------------------------*/
bool HTMU::sendParameter(bool r)
{
    static uint i, j;
    if(r == true)
    {
        i = 60000;
        j = 0;
        return false;
    }

    i += REFRESH_PERIOD;
    if(i >= 500)
    {
        i = 0;
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 6, 0x09);
        setCanData(frame,\
                   port ->pgn09SetParameter.ccmNumber,\
                   port ->pgn09SetParameter.ccmNumber >> 8,\
                   port ->pgn09SetParameter.ccmNumber >> 16,\
                   port ->pgn09SetParameter.ccmNumber >> 24,\
                   port ->pgn09SetParameter.regionalCode[0],\
                   port ->pgn09SetParameter.regionalCode[1],\
                   port ->pgn09SetParameter.regionalCode[2],\
                   0);
        Q_EMIT sendToRouting(frame);
    }

    j += REFRESH_PERIOD;
    if(j >= 5000)
    {
        j = 0;
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------------
 * Function:        sendStartCharging
 * Description:     发送启动充电帧 --- 01H
 *--------------------------------------------------------------------------*/
bool HTMU::sendStartCharging(bool r)
{
    static uint i = 0;
    i += REFRESH_PERIOD;
    if(i >= 1000)
    {
        i = 0;
        return true;
    }

    if(r == true)
    {
        i = 0;
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 4, 0x01);
        setCanData(frame,\
                   port ->id,\
                   port ->pgn01StartCharging.loadControlSwitch,\
                   0, 0, 0, 0, 0, 0);

        Q_EMIT sendToRouting(frame);
    }
    return false;
}

/*----------------------------------------------------------------------------
 * Function:        sendStartChargingCompleteResponse
 * Description:     发送启动充电完成响应帧 --- 12H
 *--------------------------------------------------------------------------*/
bool HTMU::sendStartChargingCompleteResponse(bool r, bool s)
{
    static uint i;
    if(r == true)
    {
        i = 0;
        return false;
    }

    if(s == true)
    {
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 4, 0x12);
        setCanData(frame,\
                   port ->id,\
                   port ->pgn11StartChargingCompleteResponse.loadControlSwitch,\
                   port ->pgn11StartChargingCompleteResponse.confirmID,\
                   0, 0, 0, 0, 0);
        Q_EMIT sendToRouting(frame);
        return false;
    }

    i += REFRESH_PERIOD;
    if(i >= 5000)
    {
        i = 0;
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------------
 * Function:        sendStopCharging
 * Description:     发送停止充电帧 --- 03H
 *--------------------------------------------------------------------------*/
bool HTMU::sendStopCharging(bool r)
{
    static uint i, j;
    if(r == true)
    {
        i = 60000;
        j = 0;
        return false;
    }

    i += REFRESH_PERIOD;
    if(i >= 250)
    {
        i = 0;
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 4, 0x03);
        setCanData(frame,\
                   port ->id,\
                   port ->pgn03StopCharging.stopReason,\
                   0, 0, 0, 0, 0, 0);
        Q_EMIT sendToRouting(frame);
    }

    j += REFRESH_PERIOD;
    if(j > 5000)
    {
        j = 0;
        return true;
    }
    return false;
}
/*----------------------------------------------------------------------------
 * Function:        sendStopChargingCompleteResponse
 * Description:     发送停止充电完成响应帧 --- 14H
 *--------------------------------------------------------------------------*/
bool HTMU::sendStopChargingCompleteResponse(bool r, bool s)
{
    static uint i;
    if(r == true)
    {
        i = 0;
        return false;
    }

    if(s == true)
    {
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 4, 0x14);
        setCanData(frame,\
                   port ->id,\
                   port ->pgn13StopChargingCompleteResponse.stopReason,\
                   port ->pgn13StopChargingCompleteResponse.confirmID,\
                   0, 0, 0, 0, 0);
        Q_EMIT sendToRouting(frame);
        return false;
    }

    i += REFRESH_PERIOD;
    if(i >= 5000)
    {
        i = 0;
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------------
 * Function:        sendConnectConfirmResponse
 * Description:     发送连接确认响应帧 --- 16H
 *--------------------------------------------------------------------------*/
bool HTMU::sendConnectConfirmResponse(bool s)
{
    static uint i = 0;
    i += REFRESH_PERIOD;
    if(i >= 1000)
    {
        i = 0;
        return true;
    }

    if(s == true)
    {
        i = 0;
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 4, 0x16);
        setCanData(frame,\
                   port ->id,\
                   0, 0, 0, 0, 0, 0, 0); // 成功
        Q_EMIT sendToRouting(frame);
    }
    return false;
}

/*----------------------------------------------------------------------------
 * Function:        sendLife
 * Description:     发送生命帧 --- 31H
 *--------------------------------------------------------------------------*/
void HTMU::sendLife()
{
    static uint i;
    static long life = 0x100000;
    i += REFRESH_PERIOD;
    if(i >= 1000)
    {
        i = 0;
        life ++;
        QByteArray frame;
        frame.resize(15);
        setHtmuCanId(frame, 6, 0x31);
        setCanData(frame,\
                   htcuState,\
                   port ->pgn31Life.chargeKWH,\
                   port ->pgn31Life.chargeKWH >> 8,\
                   port ->pgn31Life.chargeTime,\
                   port ->pgn31Life.chargeTime >> 8,\
                   life,\
                   life >> 8,\
                   life >> 16);
        Q_EMIT sendToRouting(frame);
    }
}
