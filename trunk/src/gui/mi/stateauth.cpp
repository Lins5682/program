#include "state.h"

AuthState::AuthState(QWidget *w,QObject *parent) :
    State(parent)
{
    passwordPage = new PasswordInput(w);
    readyPage    = new Chargeready(w);
    tip          = new Tip(w);
    currPage     = passwordPage;

    connect(passwordPage,SIGNAL(sendEditSignal(QWidget *)),
                    this,SLOT(clickLineEditSlot(QWidget *)));
    connect(passwordPage,SIGNAL(clickedConfirmButton(QString)),
                    this,SLOT(clickedConfirmButtonSlot(QString)));
    connect(passwordPage,SIGNAL(clickedCancleButton()),
                    this,SIGNAL(cancleAuth()));
    connect(passwordPage,SIGNAL(stringGenerated(QString)),
                    this,SLOT(inputPasswordHandle(QString)));

    connect(readyPage,SIGNAL(clickedCancleButton()),
                 this,SIGNAL(cancleAuth()));
    connect(readyPage,SIGNAL(clickedConfirmButton()),
                 this,SIGNAL(confirmUserInfo()));


}
void AuthState::entered()
{
    currPage->enter();
    passwordPage->setDefaultLineEditText(QObject::tr("Please click enter password"));
    passwordPage->clearPasswordLineText();
    currentFocusWidget = passwordPage->getPasswordLineWidget();
}

void AuthState::exited()
{
    currPage->exit();
}

void AuthState::clickLineEditSlot(QWidget *w)
{
    if(!w)return;
    currentFocusWidget = w;
}

void AuthState::inputPasswordHandle(const QString &s)
{
    QWidget *widget = currentFocusWidget;
    QString str;
    int k = 0;

    if (!widget) return;
    if(s == "CLR")
    {
        passwordPage->clearPasswordLineText();
        qDebug()<< passwordPage->getPasswordLineEditText();
    }
    else if(s == "BS")
    {
        str = QString();
        k = Qt::Key_Backspace;
    }
    else
    {
        str = s;
    }
    QKeyEvent keyPress(QEvent::KeyPress,  k, Qt::NoModifier, str);
    QApplication::sendEvent(widget, &keyPress);
}

void AuthState::clickedConfirmButtonSlot(QString password)
{
    emit confirmAuth(password);
    currPage->exit();
    currPage = tip;

    tip->setRemindImg(Tip::waring);
    tip->setRemind(QString::fromUtf8("正在查询，请稍候"));
    currPage->enter();
}

void AuthState::userPasswordResult(QString errorid)
{
    currPage->exit();
    if(errorid == "ok")
    {
        currPage = readyPage;
    }
    else if("1")
    {
        currPage = tip;
        tip->setRemindImg(Tip::waring);
        tip->setRemind(QString::fromUtf8("余额不足"));
    }
    currPage->enter();
}


/*
void AuthState::clickedButtonSlot(const QString &s)
{
    if(s == "confirm")
    {
        qDebug()<< passwordPage->getPasswordLineEditText();
        emit confirmPassword(passwordPage->getPasswordLineEditText());
    }
    else if(s == "cancle")
    {

    }
}
*/
