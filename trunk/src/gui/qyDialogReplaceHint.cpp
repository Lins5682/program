#include "qyDialogReplaceHint.h"

qyDialogReplaceHint :: qyDialogReplaceHint(qyDialogGroup* owned, QWidget* parent)
                     : qyDialogBase(owned, parent)
{

    tipsImg = new QLabel(this);
    tipsTxt = new QLabel(QString::fromUtf8("对不起，余额不足"), this);
}

void
qyDialogReplaceHint :: initializeDialog()
{
    tipsImg->setObjectName("tipsImg");
    tipsImg->setGeometry((size().width()  - 450) / 2,
                         (size().height() - 140) / 2,
                         65,
                         65);

    tipsImg->setProperty("type", QVariant(1));
    tipsTxt->setObjectName("tipsTxt");
    tipsTxt->setAlignment(Qt::AlignCenter);
    tipsTxt->setGeometry((size().width()  - 600) / 2,
                         (size().height() - 150) / 2 ,
                         600,
                         80);
}
