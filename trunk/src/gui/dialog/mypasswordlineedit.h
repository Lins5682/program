#ifndef MYPASSWORDLINEEDIT_H
#define MYPASSWORDLINEEDIT_H

#include "dialog/mylineedit.h"

class MyPassWordLineEdit : public MyLineEdit
{
   Q_OBJECT

private:

    QString myLineEditText;
    uint timeOut;
    uchar lastCharCount;
    QString getMaskString();

public:
    MyPassWordLineEdit(QWidget *parent,uint t = 300);
    QString realText() const;
    void clearRealText();

    void setTimeOut(uint msec);
    uint getTimeOut() const;


private slots:
    virtual void displayEditSlot(int,int);
    void displayPasswordSlot();
    void getRealTextSlot(const QString &s);
};

#endif // MYPASSWORDLINEEDIT_H
