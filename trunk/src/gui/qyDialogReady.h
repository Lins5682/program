#ifndef QYDIALOGREADY_H
#define QYDIALOGREADY_H

#include "qyDialogGroup.h"
#include <QtGui>

class qyDialogReady : public qyDialogBase
{
    Q_OBJECT
public:
                    qyDialogReady(qyDialogGroup*, QWidget*, const QString&, const QString&);
    void            initializeDialog();
private:


    QLabel*         icCardNumTip;
    QLineEdit*      icCardNumTxt;
    QLabel*         phoneNumTip;
    QLineEdit*      phoneNumTxt;
    QLabel*         cardBalanceTip;
    QLineEdit*      cardBalanceTxt;
    QLabel*         serialNumTip;
    QLineEdit*      serialNumTxt;
    QLabel*         accountBalanceTip;
    QLineEdit*      accountBalanceTxt;
    QLabel*         elecpriceTip;
    QLineEdit*      elecpriceTxt;
    QLabel*         servicepriceTip;
    QLineEdit*      servicepriceTxt;
    QLabel*         totalpriceTip;
    QLineEdit*      totalpriceTxt;
};
#endif // QYDIALOGREADY_H
