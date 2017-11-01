#ifndef QYDIALOGPASSWDAUTH_H
#define QYDIALOGPASSWDAUTH_H

#include <QtGui>
#include "qyDialogBase.h"
#include "qyDialogGroup.h"

class qyDialogPasswdAuth : public qyDialogBase
{
    Q_OBJECT
public:
    explicit        qyDialogPasswdAuth(qyDialogGroup*, QWidget*);
    void            initializeDialog();
private:
    QLabel*         progressTips;
    QProgressBar*   progress;
};

#endif // QYDIALOGPASSWDAUTH_H
