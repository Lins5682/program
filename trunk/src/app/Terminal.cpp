#include "Terminal.h"
#include <QDebug>

InterfaceCan *Terminal::intanceCan0 = 0;
Terminal::Terminal(const QString& tlName, int tlNumber, int version, int serial, const QString& code)
        : name_(tlName), exec(tlName, tlNumber), user(0), current(None)
{
    exec.initialize();
    exec.setParameter(serial, code.toAscii().at(0), code.toAscii().at(1), code.toAscii().at(2));
    exec.setHtcuVersion(version);

    if(!intanceCan0)
    {
        intanceCan0 = new InterfaceCan("can0");
        if(!intanceCan0 ->initialize())
        {
            qWarning() << "It's failed to initialize can0!";
        }
    }

    int portNumberCA, portNumberCI;
    portNumberCA = 1 + tlNumber;
    portNumberCI = 1 + tlNumber;
    Routing::instance()->registerCR(portNumberCA, exec);
    Routing::instance()->registerCR(portNumberCI, *intanceCan0);
    Routing::instance()->setRuleCR(portNumberCI, portNumberCA);
}

Terminal::~Terminal()
{
}

void Terminal::setParameters()
{
    /*TODO: Config the terminal*/
}

/*whb_debug: htmu 内部定时器启动*/
void Terminal::execute()
{
    courseCtrl.start();
    exec.moveToThread(&courseCtrl);
}

void Terminal::started()
{
    /*TODO: Change the status of terminal and generate the
     *order of terminal's user */
}

void Terminal::stoped()
{
    /*TODO: Change the status of terminal and close account */
}
