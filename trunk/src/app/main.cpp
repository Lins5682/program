#include <QApplication>
#include <QWSServer>
#include "HTCU.h"
#include "testHeaders.h"
#include "testciserialdialog.h"
#include "testcrdialog.h"
#include "../gui/qyDialogPasswdAuth.h"
#include "management.h"

#define QT_ARM_LINUX 1

int main(int argc, char *argv[])
{
    QApplication theApp(argc, argv);

#if QT_ARM_LINUX
    QWSServer::setCursorVisible(false);
#endif
    HTCU::instance()->setQss("General.qss");
    HTCU::instance()->initialize();

//    Management *m = new Management();
//    m->handle();

//    ManagementTest *mt = new ManagementTest();
//    mt->show();

    return theApp.exec();
}
