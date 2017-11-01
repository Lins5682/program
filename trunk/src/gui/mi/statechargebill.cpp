#include "state.h"

ChargebillState::ChargebillState(QWidget *w,QObject *parent)
{
    chargebill  = new ChargeBilling(w);
    welcome     = new Welcome(w);

    connect(chargebill, SIGNAL(chargebillConfirm()), this, SLOT(accountResult()));
}

ChargebillState::~ChargebillState()
{
    DELETE(chargebill);
    DELETE(welcome);
}

void ChargebillState::entered()
{
    welcome->hide();
    chargebill->show();
}

void ChargebillState::exited()
{
    chargebill->hide();
    welcome->hide();
}
