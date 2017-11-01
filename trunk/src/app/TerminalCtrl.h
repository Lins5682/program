#ifndef TERMINALCTRL_H
#define TERMINALCTRL_H

#include <QtDebug>
#include <QList>
#include <QObject>
#include <QString>
#include <QTimer>
#include "Terminal.h"

#include "routing.h"
#include "serial/interfaceserial.h"
#include "cardreader/cardreader.h"

#define g_TerminalCtrl          (TerminalCtrl::instance())
#define g_TerminalByName(name)  (TerminalCtrl::instance()->terminal(name))

class TerminalCtrl : public QObject
{
    Q_OBJECT
public:
    explicit TerminalCtrl(QObject *parent = 0);
    ~TerminalCtrl();

    Terminal* terminal(const QString name)
    {
        /*
         * TODO:Use 'QMap'
        */
        for(int i = 0; i < tlQueue.size(); i++)
        {
            if(!tlQueue.at(i)->name().compare(name))
                return tlQueue.at(i);
        }
        return 0;
    }

    void initialize();

    bool has(const QString&);

    void setFocal(QString focal)
    {
        tlFocal = focal;
    }

    void add(Terminal* terminal)
    {
        tlQueue.append(terminal);
    }

    static TerminalCtrl* instance()
    {
        if(!instance_)
            instance_ = new TerminalCtrl();
        return instance_;
    }

public slots:
    void changeFocal(int);
    void appearedSlot(const QString&);
private:
//    Routing*             route;
//    InterfaceSerial*     icCardInterface;
//    QTimer               icCardPolling;
//    CardReader           icCardReader;
    QString                 tlFocal;
    QList<Terminal*>        tlQueue;
    static TerminalCtrl*    instance_;
    QThread                 pthread;
};

#endif // TERMINALCTRL_H
