#ifndef QYDIALOGREPLACEHINT_H
#define QYDIALOGREPLACEHINT_H

#include "qyDialogGroup.h"
#include <QtGui>

class qyDialogReplaceHint : public qyDialogBase
{
    Q_OBJECT

    Q_PROPERTY(int type READ getType WRITE setType)
public:
                qyDialogReplaceHint(qyDialogGroup*, QWidget*);
    void        initializeDialog();
private:
    QLabel*     tipsImg;
    QLabel*     tipsTxt;
    int         type;
    int         getType(){return type;}
    void        setType(int e){type = e;}
};
#endif // QYDIALOGREPLACEHINT_H
