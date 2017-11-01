#include "qyDialogAccountClose.h"

qyDialogAccountClose :: qyDialogAccountClose(qyDialogGroup* owned, QWidget* parent, const QString& text,
                                                                                    const QString& hint)
                      : qyDialogBase(owned, parent, text, hint)
{

    chargeTimTip = new QLabel(QString::fromUtf8("充电时间"), this);
    chargeTimTxt = new QLineEdit(this);
    chargeKwhTip = new QLabel(QString::fromUtf8("充电电量"), this);
    chargeKwhTxt = new QLineEdit(this);
    chargeSumTip = new QLabel(QString::fromUtf8("充电电费"), this);
    chargeSumTxt = new QLineEdit(this);
    serverPriTip = new QLabel(QString::fromUtf8("服 务 费"), this);
    serverPriTxt = new QLineEdit(this);
    acountSumTip = new QLabel(QString::fromUtf8("余     额"), this);
    acountSumTxt = new QLineEdit(this);
    launchTimTip = new QLabel(QString::fromUtf8("开始时间"), this);
    launchTimTxt = new QLineEdit(this);
    finishKwhTip = new QLabel(QString::fromUtf8("结束电量"), this);
    finishKwhTxt = new QLineEdit(this);
    launchSocTip = new QLabel(QString::fromUtf8("开始SOC"), this);
    launchSocTxt = new QLineEdit(this);
    finishSocTip = new QLabel(QString::fromUtf8("结束SOC"), this);
    finishSocTxt = new QLineEdit(this);
    finishCauTip = new QLabel(QString::fromUtf8("停机原因"), this);
    finishCauTxt = new QLineEdit(this);
    chgKwhUniTip = new QLabel("Kwh", this);
    finKwhUniTip = new QLabel("Kwh", this);
    lauSocUniTip = new QLabel(  "%", this);
    finSocUniTip = new QLabel(  "%", this);
    ensureAccMsg = new QPushButton(QString::fromUtf8("确定"), this);
}


void
qyDialogAccountClose :: initializeDialog()
{
    chargeTimTip->setObjectName("chargeTimTip");
    chargeTimTxt->setObjectName("chargeKwhTxt");
    chargeKwhTip->setObjectName("chargeKwhTip");
    chargeKwhTxt->setObjectName("chargeKwhTxt");
    chargeSumTip->setObjectName("chargeSumTip");
    chargeSumTxt->setObjectName("chargeSumTxt");
    serverPriTip->setObjectName("serverPriTip");
    serverPriTxt->setObjectName("serverPriTxt");
    acountSumTip->setObjectName("acountSumTip");
    acountSumTxt->setObjectName("acountSumTxt");
    launchTimTip->setObjectName("launchTimTip");
    launchTimTxt->setObjectName("launchTimTxt");
    finishKwhTip->setObjectName("finishKwhTip");
    finishKwhTxt->setObjectName("finishKwhTxt");
    launchSocTip->setObjectName("launchSocTip");
    launchSocTxt->setObjectName("launchSocTxt");
    finishSocTip->setObjectName("finishSocTip");
    finishSocTxt->setObjectName("finishSocTxt");
    finishCauTip->setObjectName("finishCauTip");
    finishCauTxt->setObjectName("finishCauTxt");
    chgKwhUniTip->setObjectName("chgKwhUniTip");
    finKwhUniTip->setObjectName("finKwhUniTip");
    lauSocUniTip->setObjectName("lauSocUniTip");
    finSocUniTip->setObjectName("finSocUniTip");
    ensureAccMsg->setObjectName("ensureAccMsg");

    chargeTimTxt->setAlignment(Qt::AlignCenter);
    chargeKwhTxt->setAlignment(Qt::AlignCenter);
    chargeSumTxt->setAlignment(Qt::AlignCenter);
    serverPriTxt->setAlignment(Qt::AlignCenter);
    acountSumTxt->setAlignment(Qt::AlignCenter);
    launchTimTxt->setAlignment(Qt::AlignCenter);
    finishKwhTxt->setAlignment(Qt::AlignCenter);
    launchSocTxt->setAlignment(Qt::AlignCenter);
    finishSocTxt->setAlignment(Qt::AlignCenter);
    finishCauTxt->setAlignment(Qt::AlignCenter);

    chargeTimTip->setGeometry( 50, 100,  80, 20);
    chargeTimTxt->setGeometry(140, 100, 180, 27);
    chargeKwhTip->setGeometry( 50, 140,  80, 20);
    chargeKwhTxt->setGeometry(140, 140, 180, 27);
    chargeSumTip->setGeometry( 50, 180,  80, 20);
    chargeSumTxt->setGeometry(140, 180, 180, 27);
    serverPriTip->setGeometry( 50, 220,  80, 20);
    serverPriTxt->setGeometry(140, 220, 180, 27);
    acountSumTip->setGeometry( 50, 260,  80, 20);
    acountSumTxt->setGeometry(140, 260, 180, 27);
    launchTimTip->setGeometry(420, 100,  80, 20);
    launchTimTxt->setGeometry(500, 100, 180, 27);
    finishKwhTip->setGeometry(420, 140,  80, 20);
    finishKwhTxt->setGeometry(500, 140, 180, 27);
    launchSocTip->setGeometry(420, 180,  80, 20);
    launchSocTxt->setGeometry(500, 180, 180, 27);
    finishSocTip->setGeometry(420, 220,  80, 20);
    finishSocTxt->setGeometry(500, 220, 180, 27);
    finishCauTip->setGeometry(420, 260,  80, 20);
    finishCauTxt->setGeometry(500, 260, 180, 27);
    chgKwhUniTip->setGeometry(330, 140,  35, 21);
    finKwhUniTip->setGeometry(685, 140,  35, 21);
    lauSocUniTip->setGeometry(685, 180,  17, 17);
    finSocUniTip->setGeometry(685, 220,  17, 17);
    ensureAccMsg->setGeometry(325, 325, 150, 50);
    //690, 100, 16, 17
}
