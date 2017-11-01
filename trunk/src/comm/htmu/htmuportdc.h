#ifndef HTMUPORTDC_H
#define HTMUPORTDC_H

#include "interface.h"

// HTMU 直流充电端口
class HTMUPortDC
{
public:
    explicit HTMUPortDC(char number, char id);
    ~HTMUPortDC();

    char number;        // 充电枪编号
    char id;            // 协议中规定的标识
    char address;       // CAN 地址

    bool isTimeout() const;

    bool startRequest;
    bool stopRequest;

    enum State // 充电端口状态
    {
        GUN_NOT_CONNECTED,      // 充电枪未连接
        START_CHARGING_FAILURE, // 启动充电失败
        STOP_CHARGING_FAILURE,  // 停止充电失败
        START_CHARGING_COMPLETE_FAILURE,    // 启动充电完成失败
        STOP_CHARGING_COMPLETE_FAILURE,     // 停止充电完成失败
        STANDBY,    // 待机
        START_CHARGING_SUCCESS, // 启动充电成功
        STOP_CHARGING_SUCCESS,  // 停止充电成功
        START_CHARGING_COMPLETE_SUCCESS,    // 启动充电完成
        STOP_CHARGING_COMPLETE_SUCCESS      // 停止充电完成
    };
    State chargingState;

    struct // 01H* HTCUTx ---> 启动充电
    {
        uchar loadControlSwitch;    // 负荷控制开关：1-启用 2-关闭
    }pgn01StartCharging;
    struct // 02H* HTCURx <--- 启动充电应答
    {
        bool okFlag;
        bool receiveFlag;
        uchar loadControlSwitch;    // 同上
        uchar confirmID;            // 同上
    }pgn02StartChargingResponse;

    struct // 03H* HTCUTx ---> 停止充电
    {
        uchar stopReason;
    }pgn03StopCharging;
    struct // 04H* HTCURx <--- 停止充电应答
    {
        bool okFlag;
        bool receiveFlag;
        uchar confirmID;    // 同上
    }pgn04StopChargingResponse;

    struct // 06H HTCURx <--- 对时
    {
        uchar immediateExecution;   // 立即执行：0-立即 1-自行选择时间
        uchar confirmID;            // 同上
    }pgn06Time;

    struct // 08H* HTCURx <--- 通信协议版本
    {
        bool okFlag;
        bool receiveFlag;
        u_int16_t version;   // 高字节-主版本号 低字节-次版本号
    }pgn08CheckVersionResponse;
    struct // 09H* HTCUTx ---> 充电参数
    {
        long ccmNumber;         // 充电桩编号
        char regionalCode[3];   // 区域编码
    }pgn09SetParameter;
    struct // 0AH* HTCURx <--- 充电桩参数设置应答
    {
        bool okFlag;
        bool receiveFlag;
        uchar confirmID;    // 同上
    }pgn0aSetParameterResponse;

    struct // 11H* HTCURx <--- 启动充电完成
    {
        bool okFlag;
        bool receiveFlag;
        uchar loadControlSwitch;    // 2 同上
        uchar confirmID;            // 3 同上
        struct
        {
            uchar protocolVersion0; // 4 通信协议版本号
            uchar protocolVersion1;
            uchar protocolVersion2;
            uchar handshakeResult;  // 10 握手结果：0-握手成功 1-握手失败
            uint  maximumOutputVoltage; // 29 最高输出电压
            uint  minimumOutputVoltage; // 31 最低输出电压
            uint  maximumOutputCurrent; // 33 最大输出电流
        }ccm;
        struct
        {
            uchar protocolVersion0; // 7 通信协议版本号
            uchar protocolVersion1;
            uchar protocolVersion2;
            uchar batteryType;      // 11 电池类型：01H-铅酸 02H-镍氢 03H-磷酸铁锂 04H-锰酸锂
                                    //    05H-钴酸锂 06H-三元材料 07H-聚合物锂离子 08H-钛酸锂
                                    //    FFH-其他电池
            uchar maximumAllowableTemperature;      // 12 最高允许温度：1C/bit -50C -50~+200C
            uint  maximumAllowableChargingVoltage;  // 13 最高允许充电电压：0.1V/bit
            uint  maximumAllowableSingleVoltage;    // 15 最高允许单体电压：0.01/bit 0~24V
            uint  maximumAllowableChargingCurrent;  // 17 最高允许充电电流：0.1A/bit -400A
            uint  ratedVoltage;     // 19 电池额定总压：0.1V/bit
            uint  currentVoltage;   // 21 电池当前电压：0.1V/bit
            uint  ratedCapacity;    // 23 电池额定容量：0.1Ah/bit
            uint  nominalEnergy;    // 25 电池标称能量：0.1kWh/bit 0~1000kWh
            uint  soc;              // 27 电池荷电状态：0.1%/bit 0~100%
        }bms;
    }pgn11StartChargingCompleteResponse;

    struct // 13H* HTCURx <--- 停止充电完成
    {
        bool okFlag;
        bool receiveFlag;
        uchar stopReason;   // 2 停止原因
        uchar confirmID;    // 3 同上
        struct
        {
            uchar errorCode0;
            uchar errorCode1;
            uchar errorCode2;
            uchar errorCode3;
        }ccm;
        struct
        {
            uchar stopChargingReason;   // 4 中止充电原因
            uchar chargingFaultReason0; // 5 充电故障原因
            uchar chargingFaultReason1;
            uchar stopErrorReason;      // 7 中止错误原因
            uchar stopSoc;              // 8 中止荷电状态：1%/bit 0~100%
            uint  minimumSingleVoltage; // 9 电池单体最低电压：0.01V 0~24V
            uint  maximumSingleVoltage; // 11 电池单体最高电压：0.01V 0~24V
            uchar minimumTemperature;   // 13 电池最低温度：1C/bit -50~+200C
            uchar maximumTemperature;   // 14 电池最高温度：1C/bit -50~+200C
            uchar errorCode0;   // 15 超时
            uchar errorCode1;
            uchar errorCode2;
            uchar errorCode3;
        }bms;
    }pgn13StopChargingCompleteResponse;

    struct // 15H* HTCURx <--- 连接确认
    {
        bool okFlag;
        bool receiveFlag;
    }pgn15ConnectConfrm;

    struct // 21H HTCURx <--- 遥信
    {
        struct // 电池管理
        {
            bool communicationFault;    // 3 BMS通信故障
            struct // 电池
            {
                bool overCurrent;       // 6 蓄电池充电过流告警
                bool overTemperature;   // 7 蓄电池模块采样点过温告警
            }battery;
        }bms;
        struct // 充电机
        {
            uchar workingState;     // 2 工作状态：0-待机 1-工作 2-充满 3-告警 4-故障
            struct // 交流输入
            {
                bool overVoltage;   // 16 过压
                bool underVoltage;  // 17 欠压
                bool circuitBreakerFault;   // 32 断路器故障
            }input;
            struct // 直流输出
            {
                bool overVoltage;   // 4 过压
                bool underVoltage;  // 5 欠压
                bool contactorState;    // 19 接触器状态
                bool overCurrent;   // 33 过流
                bool fuseFault;     // 34 熔断器故障
            }output;
            struct // 充电枪
            {
                bool notHoming;         // 13 未归位
                bool overTemperature;   // 14 过温故障
            }gun;
            struct // 电池
            {
                bool connectedState;    // 8 连接状态
                bool reverseFault;      // 11 反接故障
            }battery;
            bool emergencyStopAction;   // 9 急停按钮动作故障
            bool insulationFault;       // 10 绝缘检测故障
            bool arresterFault;         // 12 避累器故障
            bool smokeAlarm;            // 15 烟雾报警告警
            bool controlGuidanceAlarm;  // 31 车辆控制导引告警
        }ccm;
        struct // 充电功率模块
        {
            bool fault;                 // 18 故障
            bool fanFault;              // 20 风扇故障
            bool overTemperature;       // 21 过温
            bool communicationAlarm;    // 30 通信告警
            struct // 交流输入
            {
                bool alarm;         // 22 告警
                bool overVoltage;   // 27 过压
                bool underVoltage;  // 28 欠压
                bool phaseLoss;     // 29 缺相
            }input;
            struct // 直流输出
            {
                bool shortCircuit;  // 23 短路故障
                bool overCurrent;   // 24 过流
                bool overVoltage;   // 25 过压
                bool underVoltage;  // 26 欠压
            }output;
        }cpm;
        struct // 其他类型故障
        {
            char faultNumber;
            char faultCode;
        }other;
    }pgn21RemoteDi;
    struct // 22H HTCURx <--- 遥测
    {
        struct
        {
            uint outputVoltage; // 输出电压：0.1V/bit 0~950V
            uint outputCurrent; // 输出电流：0.01A/bit 0~400A
            uint temperature;   // 环境温度：0.1C/bit -50~+90C
            uint controlGuidanceVoltage;    // 控制导引电压：0.01V/bit 0~12V
        }ccm;
        struct
        {
            uint soc;   // SOC：0.1%/bit 0~100%
            uint minimumTemperature;    // 电池最低温度：0.1C/bit -50~200C
            uint maximumTemperature;    // 电池最高温度：0.1C/bit -50~200C
            uint maximumSingleVoltage;  // 单体最高电压：0.01V/bit 0~24V
            uint minimumSingleVoltage;  // 单体最低电压：0.01V/bit 0~24V
        }bms;
    }pgn22RemoteAi;
    struct // 31H HTCUTx ---> 生命
    {
        uint chargeKWH;     // 充电电量：0.1kWH 0-1000kWH 待机时为0
        uint chargeTime;    // 充电时间：1min 0-6000min 待机时时为0
    }pgn31Life;
};

#endif // HTMUPORTDC_H
