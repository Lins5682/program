#include "qyDialogLogin.h"

qyDialogLogin :: qyDialogLogin(qyDialogGroup* group, QWidget* parent,
                               const QString& titleText, const QString& titleHint)
               : qyDialogBase(group, parent, titleText, titleHint)
{
    ensurePasswd = new QPushButton(QString::fromUtf8("确定"), this);
    cancelPasswd = new QPushButton(QString::fromUtf8("取消"), this);
    passwd       = new QLineEdit(this);
    keyboard     = new KbInstance(this);
    keyboard->move(450, 70);
}

void
qyDialogLogin :: initializeDialog()
{
    ensurePasswd->setObjectName("ensurePasswd");
    cancelPasswd->setObjectName("cancelPasswd");
    passwd->setObjectName("passwd");

    ensurePasswd->setGeometry(40, 240, 141, 51);
    cancelPasswd->setGeometry(220, 240,141, 51);
    passwd->setGeometry(40, 140, 321, 41);

    connect(ensurePasswd, SIGNAL(clicked()), this, SLOT(verifyPasswd()));
}


void
qyDialogLogin :: verifyPasswd()
{

}
