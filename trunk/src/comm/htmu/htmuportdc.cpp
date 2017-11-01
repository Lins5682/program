#include "htmuportdc.h"

/*----------------------------------------------------------------------------
 * Function:        HTMUPortDC
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
HTMUPortDC::HTMUPortDC(char number, char id)
{
    this ->number = number;
    this ->id = id;
    address = 0xf6 + number;
    chargingState = STANDBY;
}

HTMUPortDC::~HTMUPortDC()
{
}

bool HTMUPortDC::isTimeout() const
{
    return false;// code
}
