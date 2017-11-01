#include "state.h"

ConfigureState::ConfigureState(QWidget *w,QObject *parent) :
    State(parent)
{
    confirmPage = new Chargeconfirm(w);
    currPage = confirmPage;
}

ConfigureState::~ConfigureState()
{
    DELETE(confirmPage);
}

void ConfigureState::entered()
{
    confirmPage->prompt->setRemindImg(Prompt::ok);
    confirmPage->prompt->setRemind(QString::fromUtf8("连接成功，点击确认开始充电"));
    confirmPage->createCountdown(10);
    currPage->enter();
}

void ConfigureState::exited()
{
    currPage->exit();
}
