#include "qyDialogGroup.h"
#include "qyDialogGroup.h"
#include "qyDialogMain.h"
#include "qyDialogLogin.h"
#include "qyDialogPasswdAuth.h"
#include "qyDialogWarn.h"
#include "qyDialogReady.h"
#include "qyDialogLaunch.h"
#include "qyDialogRunning.h"
#include "qyDialogAccountClose.h"
#include "qyDialogReplaceHint.h"

qyDialogGroup::qyDialogGroup(const QString& name, QWidget* p = 0)
             : counter(0),
               terminalName(name),
               topDialog(eMain),
               visible(false),
               parent(p)
{
    qyDialogBase::setFixedSize(QSize(780, 440));
    dialogQueue.append(createDialog((qyDialogMain*)0,
                                    parent));

    dialogQueue.append(createDialog((qyDialogLogin*)0,
                                    parent,
                                    QString(QString::fromUtf8("密码输入")),
                                    QString("")));

    dialogQueue.append(createDialog((qyDialogPasswdAuth*)0,
                                    parent));

    dialogQueue.append(createDialog((qyDialogWarn*)0,
                                    parent,
                                    QString(QString::fromUtf8("停止充电")),
                                    QString("")));
    dialogQueue.append(createDialog((qyDialogReady*)0,
                                    parent,
                                    QString(QString::fromUtf8("用户信息")),
                                    QString("")));

    dialogQueue.append(createDialog((qyDialogLaunch*)0,
                                    parent,
                                    QString(QString::fromUtf8("启动充电")),
                                    QString(QString::fromUtf8("请将充电枪与车辆连接好"))));

    dialogQueue.append(createDialog((qyDialogRunning*)0,
                                    parent,
                                    QString(QString::fromUtf8("充电信息")),
                                    QString(QString::fromUtf8("请刷卡停机"))));

    dialogQueue.append(createDialog((qyDialogAccountClose*)0,
                                    parent,
                                    QString(QString::fromUtf8("结算信息")),
                                    QString("")));

    dialogQueue.append(createDialog((qyDialogReplaceHint*)0,
                                    parent));

    QObject::connect(g_TerminalByName(terminalName), SIGNAL(statusChanged(int)), this, SLOT(showExWithTerminal(int)));
    QObject::connect(g_TerminalByName(terminalName), SIGNAL(error(int,QString)), this, SLOT(showExAlrm(int, const QString&)));
}

qyDialogGroup::~qyDialogGroup()
{
}

void qyDialogGroup::createDialogs(QSize size)
{

}

void qyDialogGroup::showEx()
{
    // Tab状态切换时，仅仅显示当前的顶部窗口。
    visible = true;
    dialogQueue.at(topDialog)->show();
}

void qyDialogGroup::hideEx()
{
    visible = false;
    dialogQueue.at(topDialog)->hide();
}

void qyDialogGroup::showEx(int s)
{
    if (visible)    // 当前为应该显示界面组合时，隐藏前一界面，显示当前界面（默认情况下，状态是变化的！）
    {
        dialogQueue.at(topDialog)->hide();
        dialogQueue.at(s)->show();
    }
    topDialog = s;  // 无论组合应不应该显示，均将当前界面设置为顶部窗口
}

void qyDialogGroup::hideEx(int s)
{
}

void qyDialogGroup::showExAlrm(int type, const QString& msg)
{
}

void qyDialogGroup::showExWithTerminal(int tlState)
{
}

void qyDialogGroup::refresh()
{
}
