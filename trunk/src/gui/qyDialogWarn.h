#ifndef QYDIALOGWARN_H
#define QYDIALOGWARN_H

#include "qyDialogBase.h"
#include <QtGui>

class qyDialogWarn : public qyDialogBase
{
    Q_OBJECT

public:
    explicit    qyDialogWarn(qyDialogGroup*, QWidget*, const QString&, const QString&);
    void        initializeDialog();

public slots:
    void        refreshContent(const QString&);
};
#endif // QYDIALOGWARN_H
