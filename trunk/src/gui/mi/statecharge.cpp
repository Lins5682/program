#include "state.h"

ChargeState::ChargeState(QWidget *w,QObject *parent) :
    State(parent)
{
    chargeinfo = new ChargeInfo(w);
    currPage = chargeinfo;
}

ChargeState::~ChargeState()
{
    DELETE(chargeinfo);
}

void ChargeState::entered()
{
    currPage->enter();
}

