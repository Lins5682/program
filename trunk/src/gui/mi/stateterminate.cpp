#include "state.h"

TerminateState::TerminateState(QWidget *w,QObject *parent)
{
    terminate = new Terminate(w);
    currPage = terminate;
}

TerminateState::~TerminateState()
{
    DELETE(terminate);
}

void TerminateState::entered()
{
    terminate->show();
    terminate->showWaiting();
}
