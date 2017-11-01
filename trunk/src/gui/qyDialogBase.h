#ifndef QYDIALOGBASE_H
#define QYDIALOGBASE_H

#include "qyDialogGroup.h"
#include <QtGui>

class qyDialogGroup;
class qyDialogBase : public QDialog
{
    Q_OBJECT
public:
    explicit        qyDialogBase(qyDialogGroup*, QWidget*);
    explicit        qyDialogBase(qyDialogGroup*, QWidget*, const QString&, const QString&);
    static QSize    getFixedSize();
    static void     setFixedSize(const QSize& size);
    qyDialogGroup*  owned();
    void            initialize();
    virtual void    initializeDialog(){}
private:
    void            createTitle();
protected slots:
    void            showNextDialog();
    void            showPrevDialog();
protected:
    bool            isTitled;
    qyDialogGroup*  group;
    QString         tText;
    QString         tHint;
    QLabel*         titleTips;
    QLabel*         titleText;
    QLabel*         titleHint;
    QPushButton*    next; // test
    QPushButton*    prev; // test
private:
    static QSize    fixedSize;
};

#endif // QYDIALOGBASE_H
