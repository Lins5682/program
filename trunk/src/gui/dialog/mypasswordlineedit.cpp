#include "mypasswordlineedit.h"
#include <QTimer>
#include<QDebug>


MyPassWordLineEdit::MyPassWordLineEdit(QWidget *parent,uint t):
    MyLineEdit(parent)

{
    timeOut = t;
    myLineEditText = "";
    lastCharCount = 0;
//    connect(this,SIGNAL(cursorPositionChanged(int,int)),
//            this,SLOT(displayPasswordAfterEditSlot(int,int)));
    connect(this,SIGNAL(textEdited(const QString&)),
            this,SLOT(getRealTextSlot(const QString&)));

}
void MyPassWordLineEdit::displayEditSlot(int oldPos,int newPos)
{
    if(oldPos>=0 && newPos>=0)
    {
        if(newPos>oldPos)
        {
            QTimer::singleShot(timeOut,this,SLOT(displayPasswordSlot()));
        }
        else
        {
            this->setCursorPosition(oldPos);
        }
    }
}
void MyPassWordLineEdit::displayPasswordSlot(void)
{
    this->setText(getMaskString());
}

void MyPassWordLineEdit::getRealTextSlot(const QString& text)
{
    if(text.count()>lastCharCount)
    {
        myLineEditText.append(text.right(1));
    }
    else if(text.count()<lastCharCount)
    {
        myLineEditText.remove(myLineEditText.count()-1,1);
    }
    lastCharCount = myLineEditText.count();
}

QString MyPassWordLineEdit::realText()const
{
    return this->myLineEditText;
}
void MyPassWordLineEdit::clearRealText()
{
    this->myLineEditText = "";
    this->clear();
}

void MyPassWordLineEdit::setTimeOut(uint msec)
{
    timeOut = msec;
}
uint MyPassWordLineEdit::getTimeOut() const
{
   return timeOut;
}
QString MyPassWordLineEdit::getMaskString()
{
    QString mask = "";
    int count = this->text().length();
    if(count>0)
    {
        for(int i=0;i<count;i++)
        mask += "*";
    }
    return mask;
}



