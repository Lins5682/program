#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = 0);

//  void focusInEvent(QFocusEvent *event);

    void mousePressEvent(QMouseEvent *event);
    QString realText()const;
    
signals:

    void clicked(QWidget *);
    
private slots:
    virtual void displayEditSlot(int,int);

public slots:

    
};

#endif // MYLINEEDIT_H
