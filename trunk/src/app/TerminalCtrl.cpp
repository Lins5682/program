#include "TerminalCtrl.h"
#include "qyDialogGroup.h"

TerminalCtrl* TerminalCtrl::instance_ = 0;
TerminalCtrl::TerminalCtrl(QObject *parent)
            : QObject(parent)
{
    initialize();
}

TerminalCtrl::~TerminalCtrl()
{
    if(pthread.isRunning())
    {
        pthread.quit();
        pthread.wait();
    }

    qDeleteAll(tlQueue);
    tlQueue.clear();
}

void TerminalCtrl::initialize()
{
    CardReader::instance()->moveToThread(&pthread);
    QObject::connect(&pthread, SIGNAL(started()), CardReader::instance(), SLOT(poll()));
    pthread.start();
}

void TerminalCtrl::appearedSlot(const QString& cardNum)
{
    terminal(tlFocal);
}

bool TerminalCtrl::has(const QString& terminalName)
{
    for(int i = 0; i < tlQueue.size(); i++)
    {
        if(!terminalName.compare(tlQueue.at(i)->name()))
            return true;
    }
    return false;
}

void TerminalCtrl::changeFocal(int focal)
{
    if(focal < tlQueue.size())
    {
        tlFocal = focal;
        qDebug() << "The focal terminal is altered!" << ", the focal terminal is " << focal;
    }
}



