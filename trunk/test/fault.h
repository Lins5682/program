#ifndef FAULT_H
#define FAULT_H

#include <QObject>

class Fault : public QObject
{
    Q_OBJECT
/*故障码*/
#define FAULT_INSULATION                1       //绝缘故障, Insulation fault
#define FAULT_OUT_CONN_OVER_TEMP        2       //输出连接器过温故障, Output connector over temperature
#define FAULT_CONNECTOR                 3       //连接器故障, Connector
#define FAULT_BATTERY_TEMP_HIGH         4       //电池组温度过高, Battery temperature is too high
#define FAULT_HVOLTAGE_RELAY            5       //高压继电器故障, High voltage relay fault
#define FAULT_POINT_2_VOLTAGE           6       //检测点2电压检测故障, Detection point 2 voltage detection fault
#define FAULT_CURRENT_OVER              7       //电流过大, Over current
#define FAULT_VOLTAGE_ABNORMAL          8       //电压异常, Abnormal voltage
#define FAULT_CHARGER_TEMP_OVER         9       //充电机过温故障, Over temperature fault of charger
#define FAULT_CHARGER_EMERGENCY_STOP    10      //充电机急停故障, Emergency stop of charger
#define FAULT_METER                     11      //电能表故障, Electric energy meter failure
#define FAULT_HARDWARE_PDP              12      //硬件PDP故障, Hardware PDP fault
#define FAULT_HMI                       13      //HMI故障, HMI fault
#define FAULT_PWM                       14      //PWM切换故障, PWM switch fault
#define FAULT_TEMP_SENSOR               15      //温度传感器故障, Temperature sensor fault
#define FAULT_EMERGENCY_STOP            16      //急停报警, Emergency stop alarm
#define FAULT_GUN_TEMP_SENSOR           17      //充电枪温度传感器故障, Charge gun temperature sensor fault
#define FAULT_CONN_GUN_TIMEOUT          18      //连接充电枪超时, Connection charging gun timeout
#define FAULT_REV_BMS_SHUTDOWN          19      //接收BMS故障停机, Receive BMS fault shutdown
#define FAULT_PCS_BMS_COMM              20      //PCS与BMS通讯中断, PCS and BMS communication interrupt
#define FAULT_PCS_MASTER_COMM           21      //PCS与Master通讯中断, PCS and Master communication interrupt
#define FAULT_PCS_UNIT_COMM             22      //PCS与单元控制器通讯中断, PCS interrupt communication with the unit controller
#define FAULT_AC_SAMP_FAIL              23      //交流电流采样失效, AC current sampling failure
#define FAULT_DC_SAMP_FAIL              24      //直流电流采样失效, DC current sampling failure
//#define FAULT_//接收BMS故障停机, Receive BMS fault shutdown
#define FAULT_VEHICLE_CHARGING          25      //车辆禁止充电, Vehicle charging
#define FAULT_CONTACTOR                 26      //接触器故障, Contactor fault
#define FAULT_MONITOR_COMM              27      //监控系统通讯故障, Monitoring system communication failure
//#define FAULT_//防雷器告警, Lightning protection alarm
//#define FAULT_//内部过温异常, Internal over temperature anomaly
//#define FAULT_//柜体温度过高, Cabinet temperature is too high
//#define FAULT_//柜体湿度过高, Cabinet humidity is too high
//#define FAULT_//交流断路器脱扣, AC circuit breaker trip
//#define FAULT_//交流电涌保护器状态异常, Abnormal state of AC surge protector
//#define FAULT_//控制开关异常, Abnormal control switch
//#define FAULT_//烟雾传感器状态异常, Smoke sensor status
//#define FAULT_//严重漏电, Serious leakage
//#define FAULT_//一般漏电, General leakage
//#define FAULT_//控制柜环境温度传感器失效, Control cabinet temperature sensor failure
//#define FAULT_//控制柜环境湿度传感器失效, Control cabinet humidity sensor failure
//#define FAULT_//电网电压异常, Grid voltage anomaly
//#define FAULT_//控制柜环境湿度超限, Control cabinet humidity
//#define FAULT_//控制柜环境温度严重过温, The temperature of the control cabinet is too serious
//#define FAULT_//直流预充接触器回检异常, Abnormal DC DC contactor
//#define FAULT_//直流断路器回检异常, Abnormal DC circuit breaker
//#define FAULT_//直流断路器不能吸合, DC circuit breaker can not pull
//#define FAULT_//系统检查控制信号异常吸合, System check control signal
//#define FAULT_//系统检查控制信号异常断开, System check control signal abnormal disconnection
//#define FAULT_//零线接触器吸合失败, Zero line contactor
//#define FAULT_//零线接触器断开失败, Zero line contactor failure
//#define FAULT_//柜门打开, Open door
//#define FAULT_//电网频率异常, Grid frequency anomaly
//#define FAULT_//一般过载, General overload
//#define FAULT_//严重过载, Severe overload
//#define FAULT_//电池限流, Battery current limiting
//#define FAULT_//逆变器一般过温, Inverter over temperature
//#define FAULT_//交流三相电流不平衡告警, AC three-phase current unbalance alarm
//#define FAULT_//自检失败告警, Self failure alarm
//#define FAULT_//制冷设备失效, Refrigeration equipment failure
//#define FAULT_//散热器过温保护, Radiator over temperature protection
//#define FAULT_//电抗器铁芯过温保护, Reactor core over temperature protection
//#define FAULT_//直流短路保护, DC short circuit protection
//#define FAULT_//直流过压保护, DC overvoltage protection
//#define FAULT_//直流欠压保护, DC undervoltage protection
//#define FAULT_//直流反接或者未接保护, DC reverse or no protection
//#define FAULT_//直流侧断开保护, DC side off protection
//#define FAULT_//直流过流保护, DC overcurrent protection
//#define FAULT_//相序错误保护, Phase sequence error protection
//#define FAULT_//DSP保护, DSP protection
//#define FAULT_//整流电压异常保护, Abnormal voltage protection
//#define FAULT_//电流过高保护, Over current protection
//#define FAULT_//电压过高保护, Overvoltage protection
//#define FAULT_//硬件漏电保护, Hardware leakage protection

/*故障等级*/
#define LEVEL_INSULATION                1
#define LEVEL_OUT_CONN_OVER_TEMP        1
#define LEVEL_CONNECTOR                 1
#define LEVEL_BATTERY_TEMP_HIGH         1
#define LEVEL_HVOLTAGE_RELAY            1
#define LEVEL_POINT_2_VOLTAGE           1
#define LEVEL_CURRENT_OVER              1
#define LEVEL_VOLTAGE_ABNORMAL          1
#define LEVEL_CHARGER_TEMP_OVER         1
#define LEVEL_CHARGER_EMERGENCY_STOP    1
#define LEVEL_METER                     1
#define LEVEL_PWM                       1
#define LEVEL_TEMP_SENSOR               1
#define LEVEL_EMERGENCY_STOP            1
#define LEVEL_GUN_TEMP_SENSOR           1
#define LEVEL_CONN_GUN_TIMEOUT          1
#define LEVEL_REV_BMS_SHUTDOWN          1
#define LEVEL_PCS_BMS_COMM              1
#define LEVEL_PCS_MASTER_COMM           1
#define LEVEL_PCS_UNIT_COMM             1
#define LEVEL_AC_SAMP_FAIL              1
#define LEVEL_DC_SAMP_FAIL              1
//#define LEVEL_//接收BMS故障停机, Receive BMS fault shutdown
#define LEVEL_VEHICLE_CHARGING          1
#define LEVEL_CONTACTOR                 1
#define LEVEL_MONITOR_COMM              1

#define FAULT(type, level)              level << 12 | type
#define LEVEL(fault)                    fault >> 12
#define TYPE(fault)                     fault & 0x00ff

public:
    explicit Fault(int number, QObject *parent = 0);
    ~Fault();

signals:
    void addFault(int level, int type, QString msg);
    void deleteFault(int type);
public slots:
    void revExternFault(int type, bool ISFault)
    {
        u_int16_t fault = FAULT(type, faultTable[type - 1].level);

        if(ISFault == true)
        {
            if(!faultList.contains(fault))
            {
                faultList.append(fault);
                emit addFault(faultTable[type - 1].level, type, faultTable[type - 1].message);
            }
        }
        else
        {
            if(faultList.contains(fault))
            {
                faultList.removeAt(faultList.indexOf(fault));
                emit deleteFault(type);
            }
        }
    }

private:
    struct Faultstructure
    {
        uchar   type;
        uchar   level;
        QString message;
    };
    static struct Faultstructure faultTable[];
    QList<u_int16_t> faultList;
    int gunnumber;
};

#endif // FAULT_H
