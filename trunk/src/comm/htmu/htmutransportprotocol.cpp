#include "htmutransportprotocol.h"

/*----------------------------------------------------------------------------
 * Function:        HTMUTransportProtocol
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
HTMUTransportProtocol::HTMUTransportProtocol(char pf, char ps, char sa)
{
    this ->canID = ((long)pf << 16) + ((long)ps << 8) + sa;
    data.reserve(128);
    currentFrameNumber = 0;
    totalFrameNumber = 0;
    currentDataLength = 0;
    totalDataLength = 0;
}

HTMUTransportProtocol::~HTMUTransportProtocol()
{
}

bool HTMUTransportProtocol::receiveFrame(const QByteArray &frame)
{
    if(Interface::checkJ1939ID(frame, canID) != true) return false;
    int n = Interface::getCanDataChar(frame, 0);
    if((n < 1) || (n > 255)) return false;
    data = frame;
    // 首帧
    if(n == 1)
    {
        totalFrameNumber = Interface::getCanDataChar(data, 1);
        if((totalFrameNumber < 2) || (totalFrameNumber > 255)) return false;
        totalDataLength  = Interface::getCanDataInt(data, 2, true);
        if(totalDataLength < 9) return false;
        data.clear();
        data.append(Interface::getCanDataChar(data, 4));
        data.append(Interface::getCanDataChar(data, 5));
        data.append(Interface::getCanDataChar(data, 6));
        data.append(Interface::getCanDataChar(data, 7));
        currentFrameNumber = 1;
        currentDataLength = 4;
        return false;
    }
    // 检查帧顺序
    if((n - (currentFrameNumber ++)) != 1) return false;
    // 检查帧数量
    if(currentFrameNumber > totalFrameNumber) return false;
    // 保存数据
    for(int i = 1; i <= 7; i ++)
    {
        data.append(Interface::getCanDataChar(data, i));
        if((++ currentDataLength) >= totalDataLength) return true;
    }
    return false;
}

const QByteArray &HTMUTransportProtocol::getData() const
{
    return data;
}
