#include "error.h"
#include <QString>
#include <QDebug>
#include <QThread>

#define dateTimeToString(str)           str.toString("MMdd.hhmmss.zzz").toStdString().c_str()
#define CURRENT_DATE_TIME_STRING        dateTimeToString(QDateTime::currentDateTime())

bool Error::outputFlag = true;

Error::Error(const QString &name, uint sizeMaximum, bool handleFlag, bool messageFlag)
{
    if(name.size() <= 0) nameQString = QString::number((int)QThread::currentThreadId(), 16);
    else nameQString = name;
    if(nameQString.size() > ERROR_NAME_SIZE) nameQString.resize(ERROR_NAME_SIZE);
    strcpy(nameString, nameQString.toStdString().c_str());
    QDateTime t = QDateTime::currentDateTime();
    for(int i = 0; i <= ERROR_CODE_SIZE_MAX; i ++)
    {
        recode.append(new ErrorData());
        recode.at(i) ->code = 0;
        recode.at(i) ->time = t;
    }
    this ->sizeMaximum = sizeMaximum;
    position = 0;
    base = -10000;
    codeOffset = 0;
    codePosition = 0;
    timePosition = 0;
    countData = 0;
    this ->handleFlag = handleFlag;
    this ->messageFlag = messageFlag;
}

Error::~Error()
{
    for(int i = 0; i <= ERROR_CODE_SIZE_MAX; i ++) delete recode.at(i);
}

void Error::handle(bool enable)
{
    handleFlag = enable;
}

void Error::handleBase(int code)
{
    base = code;
    codeOffset = -20;
}

void Error::handle(int codeOffset)
{
    recode.at(position) ->code = base + codeOffset;
    recode.at(position) ->time = QDateTime::currentDateTime();
    if(handleFlag) qDebug("ERROR<%s>%s: NO.%lu = %d.",\
                         nameString,\
                         dateTimeToString(recode.at(position) ->time),\
                         countData, recode.at(position) ->code);
    countData ++;
    if((++ position) > ERROR_CODE_SIZE_MAX) position = 0;
    this ->codeOffset = codeOffset;
}

void Error::handle()
{
    handle(codeOffset --);
}

void Error::last(uchar number) const
{
    // code
    Q_UNUSED(number);
}

const QString &Error::name() const
{
    return nameQString;
}

int Error::code()
{
    timePosition = codePosition;
    if((-- codePosition) > ERROR_CODE_SIZE_MAX) codePosition = ERROR_CODE_SIZE_MAX;
    return recode.at(timePosition) ->code;
}

const QDateTime &Error::time() const
{
    return recode.at(timePosition) ->time;
}

void Error::refresh()
{
    codePosition = position;
    timePosition = position;
}

ulong Error::count() const
{
    return countData;
}

/*----------------------------------------------------------------------------
 * Function:        Error::message
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
void Error::message(bool enable)
{
    messageFlag = enable;
}

void Error::message(const QString &text) const
{
    message(text.toStdString().c_str());
}

void Error::messageType(Message type) const
{
    if(!messageFlag) return;
    const char *s;
    switch(type)
    {
        case INITIALIZE_START: s = "initialization start [i]"; break;
        case INITIALIZE_OK: s = "initialization ok [o]"; break;
    }
    message(s);
}

// private
void Error::message(const char *text) const
{
    if(!messageFlag) return;

    QByteArray nameba = nameQString.toUtf8();
    qWarning("MSG<%s>%s: %s",\
             nameba.data(), CURRENT_DATE_TIME_STRING,\
             text);
}
/*----------------------------------------------------------------------------
 * Function:        Error::output
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
void Error::output(bool enable)
{
    outputFlag = enable;
}

void Error::output(const QString &text, const QString &name)
{
    if(!outputFlag) return;
    QString t = text;
    QString n = name;
    if(t.size() > 512) t.resize(512);
    if(n.size() > (ERROR_NAME_SIZE + 1)) n.resize(ERROR_NAME_SIZE + 1);
    qWarning("OUT<%s>%s: %s",\
             n.toStdString().c_str(), CURRENT_DATE_TIME_STRING,\
             t.toStdString().c_str());
}
/*----------------------------------------------------------------------------
 * Function:        Error::outputHex
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
void Error::outputHex(const QByteArray &data, int number, const QString &name)
{
    if(!outputFlag) return;
    if(data.size() <= 0) return;
    if((number == 0) || (data.size() < number + 1)) number = data.size() - 1;
    if(number > 127) number = 127;
    QString s = QString("HEX[%1]").arg(number + 1);
    QString t;
    uint a;
    for(int i = 0; i <= number; i ++)
    {
        a = data.at(i);
        t = QString("%1").arg(a, 0, 16);
        if(a <= 0x0f) t = "0" + t;
        s += " " + t;
    }
    s = s.toUpper();
    s += "\0";
    output(s, name);
}
/*----------------------------------------------------------------------------
 * Function:        Error::outputCan
 * Description:
 * param[1]:
 * return:
 *--------------------------------------------------------------------------*/
void Error::outputCan(const QByteArray &data, const QString &name)
{
    if(!outputFlag) return;
    if(data.size() != 15)
    {
        output("outputCan", "can data size error.");
        return;
    }
    QString s = "HEX[CAN] ";
    QString t;
    uint a;
    int i;
    for(i = 0; i <= 3; i ++)
    {
        a = data.at(i);
        t = QString("%1").arg(a, 0, 16);
        if(a <= 0x0f) t = "0" + t;
        s += t;
    }
    a = data.at(i ++);
    s += QString(" [%1]").arg(a);
    for(; i <= 12; i ++)
    {
        a = data.at(i);
        t = QString("%1").arg(a, 0, 16);
        if(a <= 0x0f) t = "0" + t;
        s += " " + t;
    }
    a = data.at(i ++);
    t = QString("%1").arg(a, 0, 16);
    if(a <= 0x0f) t = "0" + t;
    s += " [" + t + " ";
    a = data.at(i ++);
    t = QString("%1").arg(a, 0, 16);
    if(a <= 0x0f) t = "0" + t;
    s += t + " ";
    a = ((uint)data.at(13) << 8) + data.at(14);
    if(a == 0x2020) t = "STANDARD";
    else if(a == 0x2030) t = "EXTENDED";
    else if(a == 0x2040) t = "RTR";
    else if(a == 0x20f0) t = "ERROR";
    else t = "UNKNOWN";
    s += t + "]";
    s = s.toUpper();
    s += "\0";
    output(s, name);
}
