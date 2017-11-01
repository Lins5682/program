#include "qyDialogReady.h"

qyDialogReady :: qyDialogReady(qyDialogGroup* owned, QWidget* parent, const QString& text, const QString& hint)
               : qyDialogBase(owned, parent, text, hint)
{

    icCardNumTip       = new QLabel(QString::fromUtf8("卡      号"), this);
    icCardNumTxt       = new QLineEdit(this);
    phoneNumTip        = new QLabel(QString::fromUtf8("手      机"), this);
    phoneNumTxt        = new QLineEdit(this);
    cardBalanceTip     = new QLabel(QString::fromUtf8("卡内余额"), this);
    cardBalanceTxt     = new QLineEdit(this);
    serialNumTip       = new QLabel(QString::fromUtf8("流 水 号"), this);
    serialNumTxt       = new QLineEdit(this);
    accountBalanceTip  = new QLabel(QString::fromUtf8("账户余额"), this);
    accountBalanceTxt  = new QLineEdit(this);
    elecpriceTip       = new QLabel(QString::fromUtf8("当前电价"), this);
    elecpriceTxt       = new QLineEdit(this);
    servicepriceTip    = new QLabel(QString::fromUtf8("服 务 费"), this);
    servicepriceTxt    = new QLineEdit(this);
    totalpriceTip      = new QLabel(QString::fromUtf8("总 电 价"), this);
    totalpriceTxt      = new QLineEdit(this);

}

void
qyDialogReady :: initializeDialog()
{

    icCardNumTip->setObjectName("icCardNumTip");
    icCardNumTip->setGeometry(50, 100, 80, 20);

    icCardNumTxt->setObjectName("icCardNumTxt");
    icCardNumTxt->setGeometry(140, 100,180, 27);
    icCardNumTxt->setAlignment(Qt::AlignCenter);

    phoneNumTip->setObjectName("phoneNumTip");
    phoneNumTip->setGeometry(50, 140, 80, 20);

    phoneNumTxt->setObjectName("phoneNumTxt");
    phoneNumTxt->setGeometry(140, 140,180, 27);
    phoneNumTxt->setAlignment(Qt::AlignCenter);

    cardBalanceTip->setObjectName("cardBalanceTip");
    cardBalanceTip->setGeometry(50, 180, 80, 20);

    cardBalanceTxt->setObjectName("cardBalanceTxt");
    cardBalanceTxt->setGeometry(140, 180, 180, 27);
    cardBalanceTxt->setAlignment(Qt::AlignCenter);

    serialNumTip->setObjectName("serialNumTip");
    serialNumTip->setGeometry(50, 220, 80, 20);

    serialNumTxt->setObjectName("serialNumTxt");
    serialNumTxt->setGeometry(140, 220, 180, 27);
    serialNumTxt->setAlignment(Qt::AlignCenter);

    accountBalanceTip->setObjectName("accountBalanceTip");
    accountBalanceTip->setGeometry(420, 100, 80, 20);

    accountBalanceTxt->setObjectName("accountBalanceTxt");
    accountBalanceTxt->setGeometry(500, 100, 180, 27);
    accountBalanceTxt->setAlignment(Qt::AlignCenter);

    elecpriceTip->setObjectName("elecpriceTip");
    elecpriceTip->setGeometry(420, 140, 80, 20);

    elecpriceTxt->setObjectName("elecpriceTxt");
    elecpriceTxt->setGeometry(500, 140, 180, 27);
    elecpriceTxt->setAlignment(Qt::AlignCenter);

    servicepriceTip->setObjectName("servicepriceTip");
    servicepriceTip->setGeometry(420, 180, 80, 20);

    servicepriceTxt->setObjectName("servicepriceTxt");
    servicepriceTxt->setGeometry(500, 180, 180, 27);
    servicepriceTxt->setAlignment(Qt::AlignCenter);

    totalpriceTip->setObjectName("totalpriceTip");
    totalpriceTip->setGeometry(420, 220, 80, 20);

    totalpriceTxt->setObjectName("totalpriceTxt");
    totalpriceTxt->setGeometry(500, 220, 180, 27);
    totalpriceTxt->setAlignment(Qt::AlignCenter);
}
