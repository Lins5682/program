#include "mifareone.h"

void MifareOne::receiveFromRouting(QByteArray data)
{
//    qDebug() << "data" << data.toHex();

    if(readbuf.isEmpty())
    {
        read.startbyte = data.at(0);
        read.length = (data.at(1) << 8) + data.at(2);
    }
    readbuf.append(data);
    if(read.length < readbuf.size() - 5) return;

    read.appdata.clear();;
    for(int i=0;i<readbuf.size()-5;i++)
    {
        read.appdata.append(readbuf.at(i + 3));
    }
    read.bcc = readbuf.at(readbuf.size() - 2);
    read.endbyte = readbuf.at(readbuf.size() - 1);

    ISReadEnd = true;
}

bool MifareOne::handle(int delay)
{
    /*发送数据帧*/
    u_int16_t length;
    u_int8_t bcc;
    length = data.size();
    bcc = BCC(data);
    data.insert(0, START_BYTE);
    data.insert(1, uchar(length >> 8));
    data.insert(2, uchar(length & 0x00ff));
    data.append(bcc);
    data.append(END_BYTE);

    QByteArray d;
    for(int i = 0; i < data.size(); i++)
    {
        d.append(data.at(i));
    }

    readbuf.clear();
    ISReadEnd = false;
    setStreamFrame(d);
    emit sendToRouting(d);

    /*接收数据帧*/
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->start(delay);

    QEventLoop loop;
    while(timer->isActive())
    {
        QTimer::singleShot(1, &loop, SLOT(quit()));
        loop.exec();

        if(ISReadEnd == true) timer->stop();
    }
    delete timer;

    if(!ISReadEnd) return false;

    if((read.startbyte != START_BYTE) || (read.endbyte != END_BYTE)) return false;

    if(read.bcc != BCC(read.appdata)) return false;

    status = (read.appdata.at(0) << 8) + read.appdata.at(1);
    if(status != 0x0000) return false;

    return true;
}

void MifareOne::setkeyA(QList<uchar> key)
{
    key_a.clear();
    key_a.append(key);
}

void MifareOne::setkeyB(QList<uchar> key)
{
    key_b.clear();
    key_b.append(key);
}

bool MifareOne::readCardNumber(QString &number)
{
    if(!activateContactlessCard().contains("ok")) return false;

    if(!authM1(4, "KEY_A", key_a).contains("ok")) return false;

    QList<uchar> NO;
    if(!readBlock(4, NO)) return false;

    number.clear();
    for(int i = 0; i < NO.size(); i++)
    {
        number.append(QString::number(NO.at(i) - 0x30));
    }
    return true;
}

uchar MifareOne::BCC(QList<uchar> data)
{
    uchar bcc = 0;
    for(int i=0;i<data.size();i++)
    {
        bcc = bcc xor data.at(i);
    }
    return bcc;
}

bool MifareOne::setBaudrate(int baud)
{
    data.clear();
    data.append(0x30);
    data.append(0x01);
    if(baud == 9600) data.append(0x00);
    else if(baud == 19200) data.append(0x01);
    else if(baud == 38400) data.append(0x02);
    else if(baud == 57600) data.append(0x03);
    else if(baud == 115200) data.append(0x04);
    else return false;

    if(!handle()) return false;
    return true;
}

QString MifareOne::readVersion()
{
    data.clear();
    data.append(0x31);
    data.append(0x11);

    if(!handle()) return "read version err";

    QString version;
    version = "V1.1";
    return version;
}

bool MifareOne::controlBuzzer(u_int16_t delaytime, u_int8_t times)
{
    data.clear();
    data.append(0x31);
    data.append(0x13);
    data.append(delaytime >> 8);
    data.append(delaytime & 0x00FF);
    data.append(times);

    if(!handle()) return false;
    return true;
}

bool MifareOne::controlLED(bool greenstate, bool redstate)
{
    uchar led = 0;
    if(greenstate == true) led |= 0x80;
    if(redstate == true) led |= 0x40;

    data.clear();
    data.append(0x31);
    data.append(0x14);
    data.append(led);

    if(!handle()) return false;
    return true;
}

bool MifareOne::openRadioFrequency()
{
    data.clear();
    data.append(0x31);
    data.append(0x90);


    if(!handle()) return false;
    return true;
}

bool MifareOne::closeRadioFrequency()
{
    data.clear();
    data.append(0x31);
    data.append(0x91);

    if(!handle()) return false;
    return true;
}

QString MifareOne::activateContactlessCard(u_int16_t delaytime)
{
    data.clear();
    data.append(0x32);
    data.append(0x24);
    delaytime = 0;
    data.append(delaytime >> 8);
    data.append(delaytime & 0x00FF);

    if(handle())
    {
        if(status == 0x0000)
        {
            uchar typeTmp;
            typeTmp = read.appdata.at(2);
            if(typeTmp == 0x0A)
            {
                type = "Type A";
            }
            else if(typeTmp == 0x1A)
            {
                type = "M1";
            }
            else if(typeTmp == 0x0B)
            {
                type = "Type B";
            }
            else
            {
                type = "unkown";
            }

            uid.clear();
            for(int i=0;i<read.appdata.at(3);i++)
            {
                uid.append(read.appdata.at(4+i));
            }
            return "ok";
        }
        else if(status == 0x3005)
        {
            return "fail";
        }
        else if(status == 0x3006)
        {
            return "timeout";
        }
    }
    return "err";
}

bool MifareOne::loadKey(int sector, QString keytype, QList<uchar> key)
{
    data.clear();
    data.append(0xC2);
    data.append(0x00);
    data.append(sector);
    if(keytype.contains("A"))
    {
        data.append(0x60);
    }
    else if(keytype.contains("B"))
    {
        data.append(0x61);
    }
    else
    {
        return false;
    }

    if(key.size() < 6) return "key len err";
    for(int i=0;i<6;i++)
    {
        data.append(key.at(i));
    }

    if(!handle()) return false;
    return true;
}

bool MifareOne::authKey(int block, QString keytype)
{
    data.clear();
    data.append(0xC2);
    data.append(0x01);
    data.append(block);
    if(keytype.contains("A"))
    {
        data.append(0x60);
    }
    else if(keytype.contains("B"))
    {
        data.append(0x61);
    }
    else
    {
        return false;
    }

    if(uid.size() < 4) return "uid len err";
    for(int i=0;i<4;i++)
    {
        data.append(uid.at(i));
    }

    if(!handle()) return false;
    return true;
}

QString MifareOne::authM1(int block, QString keytype, QList<uchar> key)
{
    data.clear();
    data.append(0xC2);
    data.append(0x02);
    if(keytype.contains("A"))
    {
        data.append(0x60);
    }
    else if(keytype.contains("B"))
    {
        data.append(0x61);
    }
    else
    {
        return "key type err";
    }

    data.append(block);

    if(key.size() < 6) return "key len err";
    if(uid.size() < 4) return "uid len err";

    int i;
    for(i=0;i<6;i++)
    {
        data.append(key.at(i));
    }
    for(i=0;i<4;i++)
    {
        data.append(uid.at(i));
    }

    if(handle())
    {
        if(status == 0x0000)
        {
            return "auth M1 ok";
        }
        else if(status == 0xC001)
        {
            return "auth M1 failure";
        }
        else if(status == 0xC0FF)
        {
            return "auth M1 other mistakes";
        }
    }
    return "err";
}

bool MifareOne::readBlock(int block, QList<uchar> &blockdata)
{
    data.clear();
    data.append(0xC2);
    data.append(0x03);
    data.append(block);

    if(!handle()) return false;

    blockdata.clear();
    for(int i=0;i<16;i++)
    {
        blockdata.append(read.appdata.at(2+i));
    }

    return true;
}

bool MifareOne::writeBlock(int block, QList<uchar> blockdata)
{
    data.clear();
    data.append(0xC2);
    data.append(0x04);
    data.append(block);
    if(blockdata.size() < 16)
    {
        for(int i=blockdata.size();i<16;i++)
        {
            blockdata.append(0xFF);
        }
    }
    for(int i=0;i<16;i++)
    {
        data.append(blockdata.at(i));
    }

    if(!handle()) return false;
    return true;
}

bool MifareOne::addValue(int block, int value)
{
    data.clear();
    data.append(0xC2);
    data.append(0x07);
    data.append(block);
    data.append(value);

    if(!handle()) return false;
    return true;
}

bool MifareOne::subtractValue(int block, int value)
{
    data.clear();
    data.append(0xC2);
    data.append(0x08);
    data.append(block);
    data.append(value);

    if(!handle()) return false;
    return true;
}

bool MifareOne::transmission(int block)
{
    data.clear();
    data.append(0xC2);
    data.append(0x09);
    data.append(block);

    if(!handle()) return false;
    return true;
}

bool MifareOne::recovery(int block)
{
    data.clear();
    data.append(0xC2);
    data.append(0x0A);
    data.append(block);
    if(!handle()) return false;
    return true;
}

bool MifareOne::changeKeyA(int sector, QList<uchar> keyA)
{
    data.clear();
    data.append(0xC2);
    data.append(0x05);
    data.append(sector);
    for(int i=0;i<6;i++)
    {
        data.append(keyA.at(i));
    }

    if(!handle()) return false;
    return true;
}

bool MifareOne::initBlock(int block, QList<uchar> blockdata)
{
    data.clear();
    data.append(0xC2);
    data.append(0x06);
    data.append(block);
    for(int i=0;i<4;i++)
    {
        data.append(blockdata.at(i));
    }

    if(!handle()) return false;
    return true;
}

QString MifareOne::popContactCard(u_int16_t delaytime, int cardno)
{
    data.clear();
    data.append(0x32);
    data.append(0x42);
    data.append(delaytime >> 8);
    data.append(delaytime & 0x00ff);
    data.append(cardno);
    if(handle())
    {
        if(status == 0x0000)
        {
            return "ok";
        }
        else if(status == 0x0001)
        {
            return "failure";
        }
        else if(status == 0x00F4)
        {
            return "do not support";
        }
    }
    return "err";
}
