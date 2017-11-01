#ifndef QYDIALOGMAIN_H
#define QYDIALOGMAIN_H

#include "qyDialogBase.h"
#include "dialog/code.h"

class qyDialogMain : public qyDialogBase
{
    Q_OBJECT
public:
    explicit  qyDialogMain(qyDialogGroup *group, QWidget *parent);
    void      initializeDialog();
private:
    void      generateQRCode();
private:
    QLabel*   icCard;
    QRCode*   qrCode;
    QLabel*   prompt;
};
#endif // QYDIALOGMAIN_H
