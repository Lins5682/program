#ifndef HTCU_H
#define HTCU_H

#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtDebug>
#include <QtCore/QTimer>

#include "qyMainWindow.h"
#include "SysConfig.h"
#include "TerminalCtrl.h"

#define INI_FILE_NAME   ("sysconfig.ini")   //

class HTCU
{

public:

    static HTCU* instance()
    {
        if(!instance_)
        {
            instance_ = new HTCU();
        }
        return instance_;
    }

    explicit HTCU();
    ~HTCU();

    void        initialize();
    void        setQss(const QString&);
    QStringList initializeTerminals(const QList<SysConfig::Terminal>& tlParameters);

private:
    static HTCU*        instance_;
    qyMainWindow        tlWindow;
};

#endif // HTCU_H
