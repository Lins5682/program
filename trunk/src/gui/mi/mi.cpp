#include "mi.h"
#include <QTextCodec>

mi::mi(QObject *parent) :
    QObject(parent)
{
//    this->initData();
//    this->initDialog();
//    this->initConnect();
//    displayHome();
    initialize();
    switchState(IdleTime);
}

mi::~mi()
{
    delete authstate;
    delete idlestate;
    delete configurestate;
    delete readystate;
    delete home;
    delete startstate;
    delete chargestate;
    delete terminatestate;
    delete chargebillstate;
}

void mi::initialize()
{
    activeInterfaceID   = 1;
    currentState        = 0;
    home                = new Home;
    readystate          = new ReadyState(home);
    authstate           = new AuthState(home);
    idlestate           = new IdleState(home);
    configurestate      = new ConfigureState(home);
    startstate          = new StartState(home);
    chargestate         = new ChargeState(home);
    chargebillstate     = new ChargebillState(home);
    terminatestate      = new TerminateState(home);
}

void mi::initDialog(void)
{
    currentState    = 0;
    home            = new Home;
    readystate      = new ReadyState(home);
    authstate       = new AuthState(home);
    idlestate       = new IdleState(home);
    configurestate  = new ConfigureState(home);
    startstate      = new StartState(home);
    chargestate     = new ChargeState(home);
    chargebillstate = new ChargebillState(home);
    terminatestate  = new TerminateState(home);
}

void mi::initData(void)
{
    currentState = 0;
    activeInterfaceID = 1;
}

void mi::initConnect(void)
{

    connect(home,SIGNAL(buttonClickedSingal(int)),
            this,SLOT(chooseInterfaceSlot(int)));
    connect(authstate,SIGNAL(confirmAuth(QString)),
                 this,SIGNAL(authConfirm(QString)));
    connect(authstate,SIGNAL(cancleAuth()),
                 this,SIGNAL(authCancle()));
    connect(authstate,SIGNAL(confirmUserInfo()),
                 this,SIGNAL(userInfoConfirm()));
    connect(this,SIGNAL(icCardResult(QString)),
            idlestate,SLOT(icCardResult(QString)));
    connect(this,SIGNAL(userPasswordResult(QString)),
            authstate,SLOT(userPasswordResult(QString)));
    connect(home, SIGNAL(buttonClickedSingal(int)),
            this, SLOT(chooseInterfaceSlot(int)));
    //connect(configurestate, SIGNAL(sendbuttonString(const QString)), this, SLOT(chooseProcessSlot(const QString)));

}

/*-------------------------------------------------------------
* Function:    choostInterfaceSlot
* Description: 槽函数，接收选择充电接口信号
* param[1]:    充电接口
* return:      无
* [note:]      无
*------------------------------------------------------------*/
void mi::chooseInterfaceSlot(int ch)
{
    //save current interface;
    activeInterfaceID = ch;
    emit activeInterface(ch);
}

/*-------------------------------------------------------------
* Function:    sleep
* Description: 毫秒级别非阻塞延时函数
* param[1]:    毫秒
* return:      无
* [note:]      无
*------------------------------------------------------------*/
void mi::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
/*-------------------------------------------------------------
* Function:    icCardVerifiedResult
* Description: 槽函数，IC卡校验结果
* param[1]:    错误码
* return:      无
* [note:]      无
*------------------------------------------------------------*/
void mi::icCardVerifiedResult(QString errorid)
{
    emit icCardResult(errorid);
}
/*-------------------------------------------------------------
* Function:    userPassWordVerifiedResult
* Description: 槽函数，接收用户密码认证结果
* param[1]:    错误码
* return:      无
* [note:]      无
*------------------------------------------------------------*/
void mi::userPasswordVerifiedResult(QString errorid)
{
    emit userPasswordResult(errorid);
}
/*-------------------------------------------------------------
* Function:    switchState
* Description: 槽函数，接收流程状态变化
* param[1]:    当前状态
* return:      无
* [note:]      无
*------------------------------------------------------------*/
void mi::switchState(int state)
{
    if (!currentState)
    {
        home->show();
        currentState = readystate;
        currentState->entered();
    }
    else
    {
        currentState->exited();
        switch(state)
        {
            case IdleTime:          {currentState = idlestate;          break;}
            case LaunchVerification:{currentState = authstate;          break;}
            case Configuration:     {currentState = configurestate;     break;}
            case Launch:            {currentState = startstate;         break;}
            case Operation:         {currentState = chargestate;        break;}
            case Termination:       {currentState = terminatestate;     break;}
            case Account:           {currentState = chargebillstate;    break;}
            case Fault:             {break;}
        }
        currentState->entered();
    }

//    if(!currentState)return;
//    currentState->exited();
//    switch(state)
//    {
//        case IdleTime:          {currentState = idlestate;          break;}
//        case LaunchVerification:{currentState = authstate;          break;}
//        case Configuration:     {currentState = configurestate;     break;}
//        case Launch:            {currentState = startstate;         break;}
//        case Operation:         {currentState = chargestate;        break;}
//        case Termination:       {currentState = terminatestate;     break;}
//        case Account:           {currentState = chargebillstate;    break;}
//        case Fault:             {break;}
//    }
//    currentState->entered();
}
