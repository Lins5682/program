#ifndef QYDIALOGLAUNCH_H
#define QYDIALOGLAUNCH_H

#include <QtGui>
#include "qyDialogGroup.h"

class qyDialogLaunch : public qyDialogBase
{
    Q_OBJECT
public:
                    qyDialogLaunch(qyDialogGroup*, QWidget*, const QString&, const QString&);
    void            initializeDialog();
private:
    QLabel*         tips;
    QProgressBar*   progressBar;
};

#endif // QYDIALOGLAUNCH_H
