#include "qyDialogMain.h"

qyDialogMain :: qyDialogMain(qyDialogGroup* group, QWidget* parent)
              : qyDialogBase(group, parent)
{
    icCard = new QLabel(this);
    prompt = new QLabel(this);

    QByteArray text("12345768");
    qrCode  = new QRCode(text, QSize(150, 150),this);
}

void
qyDialogMain :: initializeDialog()
{

    qrCode->setObjectName("qrCode");
    icCard->setObjectName("icCard");
    prompt->setObjectName("prompt");


    icCard->setGeometry(360, 130, 300, 100);
    prompt->setGeometry(270, 310, 300,  30);
    qrCode->setGeometry(QRect(150, 120, 150, 150));
    qrCode->setMargin (10);


    // for Test
//    icCard->setText("icCard");

    prompt->setText(QString::fromUtf8("请 扫 码 充 电 或 刷 卡 充 电"));
}

void
qyDialogMain :: generateQRCode()
{
}
