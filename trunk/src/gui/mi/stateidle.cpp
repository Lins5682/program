#include "state.h"

IdleState::IdleState(QWidget *w,QObject *parent) :
    State(parent)
{
    standbyPage = new Standby(w);
    currPage = standbyPage;
    currPage->exit();
}
void IdleState::entered()
{
    currPage->enter();
}

void IdleState::exited()
{
    currPage->exit();
}


void IdleState::icCardResult(QString errorid)
{
    //
    if(1)
    {
        standbyPage->setlabelRemindText(QString::fromUtf8("非 法 卡"));
    }
}
