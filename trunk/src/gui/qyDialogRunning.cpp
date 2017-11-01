#include "qyDialogRunning.h"

qyDialogRunning :: qyDialogRunning(qyDialogGroup* owned, QWidget* parent, const QString& text, const QString& hint)
                 : qyDialogBase(owned, parent, text, hint)
{
    icCardNumTip    = new QLabel(QString::fromUtf8("卡       号"), this);
    icCardNumTxt    = new QLineEdit(this);
    launchTimTip    = new QLabel(QString::fromUtf8("开始时间"), this);
    launchTimTxt    = new QLineEdit(this);
    accumuTimTip    = new QLabel(QString::fromUtf8("累计时间"), this);
    accumuTimTxt    = new QLineEdit(this);
    consumSumTip    = new QLabel(QString::fromUtf8("消费金额"), this);
    consumSumTxt    = new QLineEdit(this);
    chargeKwhTip    = new QLabel(QString::fromUtf8("充电电量"), this);
    chargeKwhTxt    = new QLineEdit(this);
    chargeVolTip    = new QLabel(QString::fromUtf8("充电电压"), this);
    chargeVolTxt    = new QLineEdit(this);
    chargeCurTip    = new QLabel(QString::fromUtf8("充电电流"), this);
    chargeCurTxt    = new QLineEdit(this);
    elecarSocTip    = new QLabel(QString::fromUtf8("当前SOC"), this);
    elecarSocTxt    = new QLineEdit(this);
    eleVolUniTip    = new QLabel("V", this);
    eleCurUniTip    = new QLabel("A", this);
    eleKwhUniTip    = new QLabel("Kwh", this);
    evcSocUniTip    = new QLabel("%", this);
}

void
qyDialogRunning :: initializeDialog()
{
    icCardNumTip->setObjectName("icCardNumTip");
    icCardNumTip->setGeometry(50, 100, 80, 20);

    icCardNumTxt->setObjectName("icCardNumTxt");
    icCardNumTxt->setGeometry(140, 100,180, 27);
    icCardNumTxt->setAlignment(Qt::AlignCenter);

    launchTimTip->setObjectName("launchTimTip");
    launchTimTip->setGeometry(50, 140, 80, 20);

    launchTimTxt->setObjectName("launchTimTxt");
    launchTimTxt->setGeometry(140, 140,180, 27);
    launchTimTxt->setAlignment(Qt::AlignCenter);

    accumuTimTip->setObjectName("accumuTimTip");
    accumuTimTip->setGeometry(50, 180, 80, 20);

    accumuTimTxt->setObjectName("accumuTimTxt");
    accumuTimTxt->setGeometry(140, 180, 180, 27);
    accumuTimTxt->setAlignment(Qt::AlignCenter);

    consumSumTip->setObjectName("consumSumTip");
    consumSumTip->setGeometry(50, 220, 80, 20);

    consumSumTxt->setObjectName("consumSumTxt");
    consumSumTxt->setGeometry(140, 220, 180, 27);
    consumSumTxt->setAlignment(Qt::AlignCenter);

    chargeVolTip->setObjectName("chargeVolTip");
    chargeVolTip->setGeometry(420, 100, 80, 20);

    chargeVolTxt->setObjectName("chargeVolTxt");
    chargeVolTxt->setGeometry(500, 100, 180, 27);
    chargeVolTxt->setAlignment(Qt::AlignCenter);

    chargeCurTip->setObjectName("chargeCurTip");
    chargeCurTip->setGeometry(420, 140, 80, 20);

    chargeCurTxt->setObjectName("chargeCurTxt");
    chargeCurTxt->setGeometry(500, 140, 180, 27);
    chargeCurTxt->setAlignment(Qt::AlignCenter);

    chargeKwhTip->setObjectName("chargeKwhTip");
    chargeKwhTip->setGeometry(420, 180, 80, 20);

    chargeKwhTxt->setObjectName("chargeKwhTxt");
    chargeKwhTxt->setGeometry(500, 180, 180, 27);
    chargeKwhTxt->setAlignment(Qt::AlignCenter);

    elecarSocTip->setObjectName("elecarSocTip");
    elecarSocTip->setGeometry(420, 220, 80, 20);

    elecarSocTxt->setObjectName("elecarSocTxt");
    elecarSocTxt->setGeometry(500, 220, 180, 27);
    elecarSocTxt->setAlignment(Qt::AlignCenter);

    eleVolUniTip->setObjectName("eleVolUniTip");
    eleVolUniTip->setGeometry(690, 100, 16, 17);

    eleCurUniTip->setObjectName("eleCurUniTip");
    eleCurUniTip->setGeometry(690, 140, 16, 17);

    eleKwhUniTip->setObjectName("eleKwhUniTip");
    eleKwhUniTip->setGeometry(690, 180, 35, 17);

    evcSocUniTip->setObjectName("evcSocUniTip");
    evcSocUniTip->setGeometry(690, 220, 16, 21);
}
