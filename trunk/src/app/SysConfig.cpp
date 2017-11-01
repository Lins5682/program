#include "SysConfig.h"

static SysConfig::Terminal t1 =
{
    QString::fromUtf8("枪A"),
    1,
    0x21,
    1201
};

static SysConfig::Terminal t2 =
{
    QString::fromUtf8("枪B"),
    2,
    0x22,
    1201
};

SysConfig* SysConfig::instance_ = 0;

SysConfig::SysConfig()
{
    config = new QSettings("./SysConfig.cfg", QSettings::IniFormat);
}

SysConfig::~SysConfig()
{
}

/*
 * TODO:
 * 1. Add other parameters;
 * 2. We can use other functions, such as QSettings::beginGroup(), QSettings::childGroup(),QSettings::endGroup()
*/
void SysConfig::generateDefaultConfigFile()
{
    QList<SysConfig::Terminal> tlQueue;
    tlQueue.append(t1);
    tlQueue.append(t2);
    config->beginWriteArray("Terminal");
    for(int i = 0; i < tlQueue.size(); i++)
    {
        config->setArrayIndex(i);
        config->setValue("Name"   , tlQueue.at(i).name);
        config->setValue("Number" , tlQueue.at(i).number);
        config->setValue("Address", tlQueue.at(i).address);
        config->setValue("Version", tlQueue.at(i).version);
    }
    config->endArray();
    config->sync();
}

QList<SysConfig::Terminal> SysConfig::getTerminals()
{
    QList<SysConfig::Terminal> tq;
    int size = config->beginReadArray("Terminal");
    //whb_debug
//    for (int i = 0; i < size; i++)
    for (int i = 0; i < 1; i++)
    {
        config->setArrayIndex(i);
        Terminal tl;
        tl.name     = config->value("Name").toString();
        tl.number   = config->value("Number").toInt();
        tl.address  = config->value("Address").toInt();
        tl.version  = config->value("Version").toInt();
        tq.append(tl);
    }
    config->endArray();
    return tq;
}

/*
 * TODO : Add other functions to access other parameters;
*/
