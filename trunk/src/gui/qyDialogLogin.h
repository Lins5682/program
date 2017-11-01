#ifndef QYDIALOGLOGIN_H
#define QYDIALOGLOGIN_H

#include <QtGui>
#include "qyDialogBase.h"
#include "dialog/keyboard.h"

class qyDialogLogin : public qyDialogBase
{
    Q_OBJECT
public:
    explicit        qyDialogLogin(qyDialogGroup*, QWidget*, const QString&, const QString&);
    void            initializeDialog();

public slots:
    void            verifyPasswd();
private:
    QLineEdit*      passwd;
    QPushButton*    ensurePasswd;
    QPushButton*    cancelPasswd;
    KbInstance*     keyboard;
};

#endif // QYDIALOGLOGIN_H
