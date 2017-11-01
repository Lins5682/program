#ifndef QYRUNNING_H
#define QYRUNNING_H

#include <QtGui>
#include "qyDialogGroup.h"

class qyDialogRunning : public qyDialogBase
{
    Q_OBJECT
public:
                    qyDialogRunning(qyDialogGroup*, QWidget*, const QString&, const QString&);
    void            initializeDialog();
private:

    QLabel*         eleVolUniTip;
    QLabel*         eleCurUniTip;
    QLabel*         eleKwhUniTip;
    QLabel*         evcSocUniTip;
    QLabel*         icCardNumTip;
    QLineEdit*      icCardNumTxt;
    QLabel*         launchTimTip;
    QLineEdit*      launchTimTxt;
    QLabel*         accumuTimTip;
    QLineEdit*      accumuTimTxt;
    QLabel*         consumSumTip;
    QLineEdit*      consumSumTxt;
    QLabel*         chargeVolTip;
    QLineEdit*      chargeVolTxt;
    QLabel*         chargeCurTip;
    QLineEdit*      chargeCurTxt;
    QLabel*         chargeKwhTip;
    QLineEdit*      chargeKwhTxt;
    QLabel*         elecarSocTip;
    QLineEdit*      elecarSocTxt;
};


#endif // QYRUNNING_H
