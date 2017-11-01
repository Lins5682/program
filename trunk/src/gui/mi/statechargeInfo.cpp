#include "state.h"

ChargeInfoState::ChargeInfoState(QWidget *w,QObject *parent)
{
    chargeinfo = new ChargeInfo(w);
    currPage = chargeinfo;
}

ChargeInfoState::~ChargeInfoState()
{
    if(chargeinfo)
    {
        delete chargeinfo;
        chargeinfo = NULL;
    }
}

void ChargeInfoState::entered()
{
    currPage->enter();
}
void ChargeInfoState::exited()
{
    currPage->exit();
}
