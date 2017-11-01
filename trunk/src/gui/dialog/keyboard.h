#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include <QPushButton>
#include <QWSInputMethod>
#include <QWSServer>
#include <QKeyEvent>
#include <QEvent>
#include <QPointer>
#include <QDebug>

class KeyboardBase : public QWidget
{
    Q_OBJECT
public:
    KeyboardBase(QWidget *parent = 0);
    ~KeyboardBase();

protected:
    void keyboardCreate(QList<Qt::Key> kcode, QList<QString> kunicode, int w, int h);

    void keyboardLayout(const QList<QPoint> point);

    void setKeySize(Qt::Key kcode, int w, int h);

private:
    virtual bool eventFilter(QObject *obj, QEvent *event);  //Filters events

protected:
    QList<QPushButton*> pbtn;
    QList<Qt::Key>      code;
    QList<QString>      unicode;

private:
    QWidget             *currentFocus;

private slots:
    void saveFocusWidget(QWidget*, QWidget *newFocus)
    {
        if(newFocus)
        {
            currentFocus = newFocus;
        }
    }
};

//class Keyboard : QWSInputMethod
//{
//};

class KbInstance : public KeyboardBase
{
public:
    KbInstance(QWidget *parent = 0, int w = 70, int h = 55);
    ~KbInstance();
};

#endif // KEYBOARD_H
