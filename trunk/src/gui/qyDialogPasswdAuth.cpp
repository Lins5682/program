#include "qyDialogPasswdAuth.h"

qyDialogPasswdAuth :: qyDialogPasswdAuth(qyDialogGroup* owned, QWidget* parent)
                    : qyDialogBase(owned, parent)
{
    progressTips = new QLabel(QString::fromUtf8("正在查询，请稍候"), this);
    progress     = new QProgressBar(this);
}

void
qyDialogPasswdAuth :: initializeDialog()
{
    //
    progress->setRange(0, 0);
    progress->setGeometry(
                (size().width() - 300) / 2,
                (size().height() - 20) / 2,
                 300,
                 20);
    //
    progressTips->setAlignment(Qt::AlignCenter);
    progressTips->setGeometry(
                (size().width() - 500) / 2,
                (size().height() - 20) / 2 - 10 - 30,
                500,
                30);

    // QLabel::setText(const QString&);
}
