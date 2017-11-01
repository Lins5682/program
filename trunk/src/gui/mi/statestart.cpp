#include "state.h"

StartState::StartState(QWidget *w,QObject *parent) :
    State(parent)
{
    startcharge = new StartCharge(w);
}

StartState::~StartState()
{
    DELETE(startcharge);
}

void StartState::entered()
{
    currPage = startcharge;
    startcharge->show();
    startcharge->showWaiting();
}

void StartState::startReslut(QString error)
{
    if(error != "ok")
    {
        startcharge->setWaitReslut(error);
    }
}
