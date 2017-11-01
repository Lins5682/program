#ifndef MI_H
#define MI_H

#include <QObject>
#include <QTimer>
#include "dialog/page.h"
#include "dialog/home.h"
#include "state.h"

class mi : public QObject
{
    Q_OBJECT
private:
    enum
    {
//        None,
//        Loading,
        IdleTime,
        LaunchVerification,
        Configuration,
        Launch,
        Operation,
        Termination,
        Account,
        Fault
    };
    //interface
    uint             activeInterfaceID;
    State            *currentState;
    IdleState        *idlestate;
    AuthState        *authstate;
    ConfigureState   *configurestate;
    ReadyState       *readystate;
    StartState       *startstate;
    ChargeState      *chargestate;
    ChargebillState  *chargebillstate;
    TerminateState   *terminatestate;

    Home             *home;
    //init
    void initDialog(void);
    void initConnect(void);
    void initData(void);
    //fun
    void sleep(unsigned int msec);
    //display home
    void displayHome(void);
    void enterIdel(void);
    void enterpassword(void);
    //input
    void updateInputPanelPosition();
private:
    void initialize();

public:
    explicit mi(QObject *parent = 0);
    ~mi();
//    void initConnect(ChargingFlow *cf);
private slots:

    void chooseInterfaceSlot(int ch);

signals:

    void activeInterface(int);
    //auth state   mi-->out
    void authCancle(void);
    void authConfirm(QString password);
    void userInfoConfirm(void);
    //mi-->stateauth
    void icCardResult(QString errorid);
    void userPasswordResult(QString errorid);

public slots:

    void switchState(int);
    void icCardVerifiedResult(QString errorid);      //错误代码需要修改
    void userPasswordVerifiedResult(QString errorid);//错误代码需要修改
};

#endif // MI_H
