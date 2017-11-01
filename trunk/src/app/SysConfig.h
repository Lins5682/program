#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include <QString>
#include <QtDebug>
#include <QSettings>

class SysConfig
{
public:

    struct Terminal                         // 终端（监控单元）信息
    {
        QString     name;                   // 终端名称
        int         number;                 // 终端编号
        int         address;                // 终端地址
        int         version;                // 终端协议版本号

    };

    static SysConfig* instance()
    {
        if(!instance_)
        {
            instance_ = new SysConfig();
        }
        return instance_;
    }

    explicit SysConfig();
    ~SysConfig();

    void generateDefaultConfigFile();

    QList<Terminal> getTerminals();

private:
    QSettings*         config;
    static SysConfig*  instance_;
};

#endif // SYSCONFIG_H
