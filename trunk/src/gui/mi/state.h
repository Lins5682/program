#ifndef STATE_H
#define STATE_H

#include <QObject>
#include "dialog/page.h"
#include "dialog/chargeconfirm.h"
#include "dialog/chargeready.h"
#include "dialog/welcome.h"
#include "dialog/standby.h"
#include "dialog/chargeinfo.h"
#include "dialog/chargebilling.h"
#include "dialog/inputpanel.h"
#include "dialog/passwordinput.h"
#include "dialog/terminate.h"
#include "dialog/tip.h"
#include "dialog/startcharge.h"

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0)
    {
    }

    virtual ~State(){}

    virtual void entered(){}
    virtual void exited()
    {
        currPage->exit();
    }

protected:
    void changePage(Page *page)
    {
        currPage->hide();
        currPage = page;
        currPage->show();
    }

    Page *currPage;

signals:

private:

public slots:

};

class IdleState :public State
{
    Q_OBJECT
private:
    Standby *standbyPage;

public:
    explicit IdleState(QWidget *w,QObject *parent = 0);
    virtual ~IdleState()
    {
        delete standbyPage;
    }
    virtual void entered();
    virtual void exited();

signals:

public slots:
    void icCardResult(QString errorid);

};


class AuthState :public State
{
    Q_OBJECT
private:
    PasswordInput      *passwordPage;
    Tip                *tip;
    Chargeready        *readyPage;
    QWidget            *currentFocusWidget;


public:

    explicit AuthState(QWidget *w,QObject *parent = 0);
    virtual ~AuthState()
    {
        delete passwordPage;
    }
    virtual void entered();
    virtual void exited();

signals:
    void confirmPassword(QString);

    void confirmAuth(QString);
    void cancleAuth();
    void confirmUserInfo();

private slots:

    void clickLineEditSlot(QWidget *w);
    void clickedConfirmButtonSlot(QString password);
    void inputPasswordHandle(const QString &s);

public slots:

    void userPasswordResult(QString errorid);


};

class ConfigureState :public State
{
    Q_OBJECT
private:
    Chargeconfirm    *confirmPage;

public:
    explicit ConfigureState(QWidget *w,QObject *parent = 0);
    virtual ~ConfigureState();
    virtual void entered();
    virtual void exited();

signals:


private slots:


public slots:


};

class ReadyState :public State
{
    Q_OBJECT
private:

    Chargeready    *readyPage;

public:
    explicit ReadyState(QWidget *w,QObject *parent = 0);
    virtual ~ReadyState()
    {
        delete readyPage;
    }
    virtual void entered();
    virtual void exited();

signals:


private slots:


public slots:


};

class StartState : public State
{
    Q_OBJECT
private:
    StartCharge *startcharge;

public:
    explicit StartState(QWidget *w, QObject *parent = 0);
    ~StartState();
    virtual void entered();

private slots:
    void startReslut(QString error);
};

class ChargeState : public State
{
    Q_OBJECT
private:
    ChargeInfo  *chargeinfo;

public:
    explicit ChargeState(QWidget *w, QObject *parent = 0);
    ~ChargeState();

    virtual void entered();
};

class TerminateState : public State
{
    Q_OBJECT
private:
    Terminate *terminate;

public:
    explicit TerminateState(QWidget *w, QObject *parent = 0);
    ~TerminateState();

    virtual void entered();

private slots:
    void stopResult(QString err)
    {
        if(err != "ok")
        {
            terminate->setStopReslut(err);
            terminate->showWaitReslut();
        }
    }
};

class ChargebillState : public State
{
    Q_OBJECT
private:
    ChargeBilling   *chargebill;
    Welcome         *welcome;

public:
    explicit ChargebillState(QWidget *w, QObject *parent = 0);
    ~ChargebillState();

    virtual void entered();
    virtual void exited();

signals:

private slots:
    void accountResult()
    {
        chargebill->hide();
        welcome->show();
    }
};

#endif // STATE_H
