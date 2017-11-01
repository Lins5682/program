#include "interface.h"

#define ADDITIONAL_DATA_LENGTH      2 // @ N+0

Interface::Interface(Type type, const QString &name, QObject *parent) :
    QObject(parent)
{
    faultFlag = false;
    QString nameTemp;
    if((type == TYPE_ERROR) || (type == CI_MAX) || (type == CA_MAX))
    {
        type = TYPE_ERROR;
        faultFlag = true;
    }
    if(name.size() <= 0)
    {
        nameTemp = "NAME_ERROR";
        faultFlag = true;
    }
    else if(name.size() > 16)
    {
        nameTemp = name;
        nameTemp.resize(16);
        faultFlag = true;
    }
    else
    {
        nameTemp = name;
    }
    this ->type = type;
    this ->name = nameTemp.toUpper();
    if(type < CI_MAX) connectedFlag = true;
    else if(type < CA_MAX) connectedFlag = true;
    else connectedFlag = false;
    initializedFlag = false;
    availableFlag = false;
    readTimer = new QTimer();
    error = new Error(this ->name);
    error ->handleBase(ERROR_PARAMETER);
    if(faultFlag) error ->handle(-1);
}

Interface::~Interface()
{
    delete error;
    delete readTimer;
}

/*----------------------------------------------------------------------------
 * Function:        set data
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
bool Interface::setInt(QByteArray &data, int i, int d, bool lsb)
{
    if(data.size() < i + 1 + 1) return false;
    if(lsb)
    {
        data[i ++] = d;
        data[i] = d >> 8;
    }
    else
    {
        data[i ++] = d >> 8;
        data[i] = d;
    }
    return true;
}

void Interface::appendInt(QByteArray &data, int d)
{
    data.append(d >> 8);
    data.append(d);
}

bool Interface::setLong(QByteArray &data, int i, long d, bool lsb)
{
    if(data.size() < i + 1 + 3) return false;
    if(lsb)
    {
        data[i ++] = d;
        data[i ++] = d >> 8;
        data[i ++] = d >> 16;
        data[i] = d >> 24;
    }
    else
    {
        data[i ++] = d >> 24;
        data[i ++] = d >> 16;
        data[i ++] = d >> 8;
        data[i] = d;
    }
    return true;
}

bool Interface::setLong(QByteArray &data, int i, char h0, char h1, char l0, char l1, bool lsb)
{
    return setLong(data, i, ((long)h0 << 24) + ((long)h1 << 16) + ((long)l0 << 8) + l1, lsb);
}

int Interface::getInt(const QByteArray &data, int i, bool lsb)
{
    if(data.size() < i + 1 + 1) return -1;
    if(lsb) return data[i] + ((int)data[i + 1] << 8);
    return ((int)data[i] << 8) + data[i + 1];
}

long Interface::getLong(const QByteArray &data, int i, bool lsb)
{
    if(data.size() < i + 1 + 3) return -1;
    if(lsb) return data[i] + ((long)data[i + 1] << 8) +\
                   ((long)data[i + 2] << 16) + ((long)data[i + 3] << 24);
    return ((long)data[i] << 24) + ((long)data[i + 1] << 16) +\
           ((long)data[i + 2] << 8) + ((long)data[i + 3]);
}

/*----------------------------------------------------------------------------
 * Function:        frame
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
bool Interface::checkFrame(const QByteArray &data, Frame type)
{
    int l = data.size(); // @ N+0
    // 无数据
    if(l < (ADDITIONAL_DATA_LENGTH + 1)) return false;
    // 帧类型
    int t = ((int)data.at(l - 2) << 8) + data.at(l - 1); // @ N+1
    if((t == STANDARD) || (t == EXTENDED) || (t == RTR) || (t == ERROR)) t = CAN;
    // 帧类型不符
    if(t != type) return false;
    // CAN 帧
    if(type == CAN)
    {
        // data - IADL 长度，固定： ID0-ID3 + LEN + D0-D7
        if(l != 15) return false; // @ N+0
        // CAN 数据域长度检查
        if(data.at(4) > 8) return false; // @ N+1 N+0
    }
    return true;
}

void Interface::setStreamFrame(QByteArray &data)
{
    appendInt(data, STREAM);
}

void Interface::removeFrame(QByteArray &data)
{
    if(data.size() < ADDITIONAL_DATA_LENGTH + 1) data.clear();
    else data.resize(data.size() - ADDITIONAL_DATA_LENGTH);
}

/*----------------------------------------------------------------------------
 * Function:        can
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
bool Interface::checkCanFrame(const QByteArray &data, Interface::CanFrame type)
{
    if(!checkFrame(data, CAN)) return false;
    int t = ((int)data.at(data.size() - 2) << 8) + data.at(data.size() - 1); // @ N+1
    if(t != type) return false;
    return true;
}

bool Interface::setCanId(QByteArray &data, long id, CanFrame type)
{
    if(data.size() < 15) return false; // @ N+0
    setLong(data, 0, id);
    data[0] = data.at(0) & 0b00011111;
    data[4] = 0;
    if(type == EXTENDED)    data[0] = data.at(0) | 0b10000000;
    else if(type == RTR)    data[0] = data.at(0) | 0b01000000;
    else if(type == ERROR)  data[0] = data.at(0) | 0b00100000;
    // else STANDARD
    setInt(data, 13, type);
    return true;
}

bool Interface::setCanData(QByteArray &data, char d0, char d1, char d2,\
                           char d3, char d4, char d5, char d6, char d7)
{
    if(data.size() < 13) return false;
    data[4] = 8;
    data[5] = d0;
    data[6] = d1;
    data[7] = d2;
    data[8] = d3;
    data[9] = d4;
    data[10] = d5;
    data[11] = d6;
    data[12] = d7;
    return true;
}

bool Interface::setCanData(QByteArray &data, int d0, int d1, int d2, int d3, bool lsb)
{    
    if(data.size() < 13) return false;
    if(lsb)
    {
        data[4] = 8;
        data[5] = d0;
        data[6] = d0 >> 8;
        data[7] = d1;
        data[8] = d1 >> 8;
        data[9] = d2;
        data[10] = d2 >> 8;
        data[11] = d3;
        data[12] = d3 >> 8;
    }
    else
    {
        data[4] = 8;
        data[5] = d0 >> 8;
        data[6] = d0;
        data[7] = d1 >> 8;
        data[8] = d1;
        data[9] = d2 >> 8;
        data[10] = d2;
        data[11] = d3 >> 8;
        data[12] = d3;
    }
    return true;
}

char Interface::getCanLength(const QByteArray &data)
{
    if(data.size() < 13) return -1;
    return data.at(4);
}

char Interface::getCanDataChar(const QByteArray &data, int i)
{
    if(data.size() < 13) return -1;
    if(data.size() < i + 1 + 5) return -1;
    return data.at(i + 5);
}

int Interface::getCanDataInt(const QByteArray &data, int i, bool lsb)
{
    if(data.size() < 13) return -1;
    return getInt(data, i + 5, lsb);
}

long Interface::getCanDataLong(const QByteArray &data, int i, bool lsb)
{
    if(data.size() < 13) return -1;
    return getLong(data, 5 + i, lsb);
}

uchar Interface::getCanDataBCD(const QByteArray &data, int i)
{
    if(data.size() < 13) return -1;
    return  ((data.at(i) >> 4) & 0x0f) * 10 + (data.at(i) & 0x0f);
}

u_int8_t Interface::getBCD(u_int8_t d)
{
    return  ((((d / 10) << 4) & 0xf0) + (d % 10));
}

/*----------------------------------------------------------------------------
 * Function:        j1939
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
bool Interface::checkJ1939PSSA(const QByteArray &data, char ps, char sa)
{
    if(data.size() < 13) return false;
    if((data.at(2) != ps) || (data.at(3) != sa)) return false;
    return true;
}

bool Interface::checkJ1939ID(const QByteArray &data, long id)
{
    if(data.size() < 13) return false;
    if((getLong(data, 0) & 0x00ffffff) != (id & 0x00ffffff)) return false;
    return true;
}

bool Interface::setJ1939Id(QByteArray &data, char p, char pf, char ps, char sa)
{
    return setCanId(data, ((long)p << 26) + ((long)pf << 16) + ((long)ps << 8) + sa, EXTENDED);
}

long Interface::getJ1939Id(const QByteArray &data)
{
    if(data.size() < 13) return -1;
    return getLong(data, 0) & 0x00ffffff;
}

long Interface::getJ1939PGN(const QByteArray &data)
{
    if(data.size() < 13) return -1;
    return (getLong(data, 0) & 0x00ff0000) >> 8;
}

/*----------------------------------------------------------------------------
 * Function:        Interface::initialize
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
bool Interface::initialize()
{
    error ->handleBase(ERROR_INITIALIZE);
    // 有故障
    if(faultFlag)
    {
        error ->handle(-1);
        return false;
    }
    // 已初始化
    if(initializedFlag)
    {
        error ->handle(-2);
        return false;
    }
    return true;
}

void Interface::receiveFromRouting(QByteArray data)
{
    Q_UNUSED(data);
}

void Interface::connectChanged(bool connect)
{
    Q_UNUSED(connect);
}

void Interface::setConnect(bool connect)
{
    connectedFlag = connect;
    Q_EMIT connectChangedSignal(connect);
}
