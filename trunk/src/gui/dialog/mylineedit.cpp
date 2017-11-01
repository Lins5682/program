#include "mylineedit.h"
#include <QDebug>
#include <QMouseEvent>

MyLineEdit::MyLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,SIGNAL(cursorPositionChanged(int,int)),
            this,SLOT(displayEditSlot(int,int)));
}



//void MyLineEdit::focusInEvent(QFocusEvent *event)
//{

//     emit editSignal(this);
//     clearFocus();
//     QLineEdit::focusInEvent(event);
//}


void MyLineEdit::mousePressEvent(QMouseEvent *event)
{
    QString s;
    if (event->button() == Qt::LeftButton)
    {
        emit clicked(this);
    }

    QLineEdit::mousePressEvent(event);
}

void MyLineEdit::displayEditSlot(int oldPos,int newPos)
{
    if(oldPos>=0 && newPos>=0 )
    {
        this->setCursorPosition(newPos > oldPos ? newPos : oldPos);
    }
}

QString MyLineEdit::realText()const
{
    return text();
}
