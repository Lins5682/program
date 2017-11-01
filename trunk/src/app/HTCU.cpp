#include "HTCU.h"

HTCU* HTCU::instance_ = 0;
HTCU::HTCU()
{
}

HTCU::~HTCU()
{
}

void HTCU::setQss(const QString& qssFileName)
{
    QFile qssFile(qssFileName);
    if(!qssFile.open(QFile::ReadOnly))
        qWarning() << "It's failed to open QSS file -- " << qssFileName;
    qApp->setStyleSheet(qssFile.readAll());
    qssFile.close();
}

QStringList HTCU::initializeTerminals(const QList<SysConfig::Terminal>& tlParameters)
{
    QStringList tlNames;
    for(int i = 0; i < tlParameters.size(); i++)
    {
        QString name = tlParameters.at(i).name;
        if(!g_TerminalCtrl->has(name))
        {
            Terminal* tl = new Terminal(name);d
            g_TerminalCtrl->add(tl);
            tlNames.append(name);
        }
    }
    return tlNames;
}

void HTCU::initialize()
{
    QStringList tlNames = initializeTerminals(SysConfig::instance()->getTerminals());
    if (tlNames.empty())
        SysConfig::instance()->generateDefaultConfigFile();
    tlWindow.initialize(tlNames);
    tlWindow.show();
}
