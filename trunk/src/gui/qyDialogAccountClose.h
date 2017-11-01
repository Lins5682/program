#ifndef QYDIALOGACCOUNTCLOSE_H
#define QYDIALOGACCOUNTCLOSE_H

#include "qyDialogGroup.h"
#include <QtGui>


class qyDialogAccountClose : public qyDialogBase
{
    Q_OBJECT
public:
                qyDialogAccountClose(qyDialogGroup*, QWidget*, const QString&, const QString&);
    void        initializeDialog();

private:
     QLabel*        chargeTimTip;
     QLineEdit*     chargeTimTxt;
     QLabel*        chargeKwhTip;
     QLineEdit*     chargeKwhTxt;
     QLabel*        chargeSumTip;
     QLineEdit*     chargeSumTxt;
     QLabel*        serverPriTip;
     QLineEdit*     serverPriTxt;
     QLabel*        acountSumTip;
     QLineEdit*     acountSumTxt;
     QLabel*        launchTimTip;
     QLineEdit*     launchTimTxt;
     QLabel*        finishKwhTip;
     QLineEdit*     finishKwhTxt;
     QLabel*        launchSocTip;
     QLineEdit*     launchSocTxt;
     QLabel*        finishSocTip;
     QLineEdit*     finishSocTxt;
     QLabel*        finishCauTip;
     QLineEdit*     finishCauTxt;
     QLabel*        chgKwhUniTip;
     QLabel*        finKwhUniTip;
     QLabel*        lauSocUniTip;
     QLabel*        finSocUniTip;
     QPushButton*   ensureAccMsg;
};

#endif // QYDIALOGACCOUNTCLOSE_H
