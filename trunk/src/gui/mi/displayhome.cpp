#include "mi.h"

void mi::displayHome(void)
{
    this->home->show();
    switchState(IdleTime);
}
