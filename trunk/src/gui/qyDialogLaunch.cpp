#include "qyDialogLaunch.h"

qyDialogLaunch :: qyDialogLaunch(qyDialogGroup* owned, QWidget* parent,const QString& text, const QString& hint)
                : qyDialogBase(owned, parent, text, hint)
{
    tips        = new QLabel(QString::fromUtf8("请等待..."), this);
    progressBar = new QProgressBar(this);
}

void
qyDialogLaunch :: initializeDialog()
{
    tips->setObjectName("tips");
    tips->setGeometry(
                 (size().width() - 300) / 2,
                 (size().height() - 20) / 2,
                 300,
                 20);

    progressBar->setRange(0, 0);
    progressBar->setObjectName("progressBar");
    progressBar->setGeometry(
                 (size().width() - 300 - 100) / 2,
                 (size().height() - 20 - 100) / 2,
                 400,
                 20);
}
