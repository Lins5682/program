#include "qyDialogWarn.h"

qyDialogWarn :: qyDialogWarn(qyDialogGroup* group,QWidget* parent,
                             const QString& titleText,const QString& titleHint)
              : qyDialogBase(group, parent, titleText, titleHint)
{
}

void
qyDialogWarn :: initializeDialog()
{
}

void
qyDialogWarn :: refreshContent(const QString& content)
{
}
