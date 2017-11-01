
#include "state.h"

ReadyState::ReadyState(QWidget *w,QObject *parent) :
    State(parent)
{
    readyPage = new Chargeready(w);
    currPage = readyPage;
}
void ReadyState::entered()
{
    currPage->enter();
}

void ReadyState::exited()
{
    currPage->exit();
}
