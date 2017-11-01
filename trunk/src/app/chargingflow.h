#ifndef CHARGINGFLOWCONTROLLER_H
#define CHARGINGFLOWCONTROLLER_H

#include <QObject>
#include <QList>
#include <QQueue>
#include <QString>
//#include "mi.h"

class ChargingFlow;

class ChargingFlowState
{
protected:

    int         id;                         // 状态ID
//    bool        alive;
public:

    // State ID
    enum
    {
        IdleTime,                           // 闲置
        Verifying,                          // 验证
        Configurating,                      // 配置
        Launching,                          // 启动
        Operating,                          // 运行
        Terminating,                        // 停止
        AccountClosing,                     // 结算
        Fault                               // 错误
    };

    // Message Type
    enum{
        VerifyIcCard,                       // 检验IC卡片
        NotifyIcCardVerificationResults,    // 通知IC卡片检验结果

        VerifyPasswd,                       // 验证口令
        CancelPasswdVerification,           // 取消口令验证
        NotifyPasswdVerificationResults,    // 通知口令验证结果

        Config,                             // 配置
        CancelConfiguration,                // 取消配置

        NotifyConnectionResults,            // 通知连接结果
        Launch,                             // 启动
        NotifyLaunchResults,                // 通知启动结果

        Terminate,                          // 终止
        NotifyTerminationResults,           // 通知终止结果

        NotifyAccountClosingResults,        // 通知结帐结果
        EnsureAccountClosing,               // 确定结帐
        CloseAccount                        // 结帐
    };

    ChargingFlowState(int i):id(i)
    {

    }

    inline void setID(int id)
    {
        this->id = id;
    }

    inline int getID()
    {
        return id;
    }

    virtual void exit(ChargingFlow* flow)
    {

    }

    virtual void enter(ChargingFlow* flow)
    {

    }

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData)
    {
        return 0;
    }
};

class IdletimeState : public ChargingFlowState
{
public:

    IdletimeState();

//    virtual void exit(ChargingFlow* flow){}

//    virtual void enter(ChargingFlow* flow){}

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);

private:

    QString isValidCard(QString* icCardNumber);
};

class VerificationState : public ChargingFlowState
{
public:

    VerificationState();

//    virtual void exit(ChargingFlow* flow);

//    virtual void enter(ChargingFlow* flow);

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);

private:
    QString isValidPassword(QString* password);
};

class ConfigurationState : public ChargingFlowState
{
public:

    ConfigurationState();

//    virtual void exit(ChargingFlow* flow);

//    virtual void enter(ChargingFlow* flow);

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);
};

class LaunchState : public ChargingFlowState
{
public:

    LaunchState();

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);

//    virtual void exit(ChargingFlow* flow);

    virtual void enter(ChargingFlow* flow);

};

class OperationState : public ChargingFlowState
{
public:

    OperationState();

//    virtual void exit(ChargingFlow* flow);

//    virtual void enter(ChargingFlow* flow);

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);

private:

    QString isValidCard(QString* number);

    bool isValidTerminationCommand(/*停止信息*/);
};

class TerminationState : public ChargingFlowState
{
public:

    TerminationState();

//    virtual void exit(ChargingFlow* flow);

    virtual void enter(ChargingFlow* flow);

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);
};

class  AccountState : public ChargingFlowState
{
public:

    AccountState();

//    virtual void exit(ChargingFlow* flow);

    virtual void enter(ChargingFlow* flow);

    virtual ChargingFlowState* handle(ChargingFlow* flow, int messageType, void* messageData);
};

class ChargingFlow : public QObject
{
    Q_OBJECT
private:
    int                 id;                         // 流程标识
    bool                alive;                      // 流程激活标志 true: 前台   false: 后台
    ChargingFlowState*  currState;                  // 当前运行状态

public:
    // 构造
    ChargingFlow() : currState(new IdletimeState)
    {
    }
    ~ChargingFlow()
    {
    }
    // 设置id
    inline void setID(int id)
    {
        this->id = id;
    }
    // 获取id
    inline int getID()
    {
        return id;
    }
//    // 获取alive，是否为前台流程？
//    inline bool getAlive()
//    {
//        return alive;
//    }
//    // 设置alive，设置当前流程为前台流程还是后台流程？
//    inline void setAlive(bool aliveFlag)
//    {
//        alive = aliveFlag;
//    }
    // 外部产生的消息，通用处理
    void generalHandleFromOuter(int messageType, void* messageData);
    // 外部产生的消息，若流程为激活状态时，进行处理
    void ifAliveHandleFromOuter(int messageType, void* messageData);
    // 内部产生的消息，通用处理
    void generalHandleFromInner(int messageType, void* messageData);
    //
//    void generalHandle(int messageType, void* messageData);
//    //
//    void ifAliveHandle(int messageType, void* messageData);

protected slots:
    //    // 验证卡片
    //    void CardVerification(QString* number);
    //    // 验证密码
    //    void verifyPasswd(QString* passWord);
    //    // 远端已验证
    //    void PasswdVerifiedByRemote(QString* detail);
    //    // 取消密码验证
    //    void cancelPasswdVerification();
    //    // 确认配置
    //    void ensureConfigure();
    //    // 取消配置
    //    void ConfigurationToCancel();
    //    // 启动充电
    //    void launch(int whichOne, QString serialNumber);
    //    // 停止充电
    //    void terminate(){}
    //    // 停止充电
    //    void terminate(int whichOne);
    //    // 已结算完成
    //    void accounted(){}
    //    // 已确认结算信息
    //    void ensureAccount();

    // UI切换了激活页，流程重新设置激活属性
    void resetActive(int id);
    // 设备检测到卡片，流程进行卡片验证
    void verifyIcCard(QString number);
    // UI确定口令验证，流程进行卡片验证
    void verifyPasswd(QString passwd);
    // UI取消口令验证，流程取消卡片验证
    void cancelPasswdVerification();
    // 后台已进行口令验证，流程告知口令验证结果
    void notifyPasswdVerificationResults(QString results);
    // UI取消了配置，流程取消配置
    void cancelConfiguration();
    // UI确定了配置，流程进行配置
    void config();
    // 监控检测到连接，流程告知UI连接结果
    void notifyConnectionResults(QString results);
    // 后台确定启动，流程进行启动
    void launch(int whichOne, QString serialNumber);
    // 监控已进行启动，流程告知UI启动结果
    void notifyLaunchResults(QString results);
    // 外部确定停止，流程进行停止
    void terminate(int whichOne);
    // 监控已进行停止，流程告知UI停止结果
    void notifyTerminationResults(QString results);
    // 后台已进行结算，流程告知UI结算结果
    void notifyAccountClosingResults(QString results);
    // UI确定结算结果，流程停止结算
    void finishAccountClosing();
    //

signals:
    // 告知UI卡片已验证
    void icCardVerified(QString cardNumb);
    // 告知UI口令已验证
    void passwdVerified(QString passWord);
    // 告知后台UI要进行口令验证
    void passwdVerificationDetermined(QString error);
    // 告知UI充电枪已连接
    void connected(QString error);
    // 告知监控UI要进行启动
    void launchDetermined(int);
    // 告知UI监控已进行启动
    void launched(QString error);
    // 告知监控UI要进行停止
    void terminationDetemined(int);
    // 告知UI监控已进行停止
    void terminated(QString error);
    // 告知后台UI要进行结帐
    void accountClosingDetermined();
    // 告知UI后台要已进行结帐
    void accountClosed();
    // 告知UI状态已发生改变
    void stateChanged(int);
//    // 卡片已验证
//    void CardVerified(QString* detail);
//    // 密码已验证
//    void passwdVerified(QString* detail);
//    // 远端验证密码
//    void remoteVerifyPasswd(QString* passwd);
//    // 物理连接
//    void connected();
//    // 启动充电
//    void launch();
//    // 启动充电已经完成
//    void launched(QString* detail);
//    // 停止充电已经完成
//    void terminated();
//    // 进行结算
//    void account();
};

#endif // CHARGINGFLOWCONTROLLER_H
