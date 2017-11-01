#include "keyboard.h"

using namespace Qt;

KeyboardBase::KeyboardBase(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    currentFocus = 0;
    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(saveFocusWidget(QWidget*, QWidget*)));
}

KeyboardBase::~KeyboardBase()
{
    qDeleteAll(pbtn);
    pbtn.clear();
    code.clear();
    unicode.clear();
}

bool KeyboardBase::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::MouseButtonPress:
        {
            if(currentFocus != 0)
            {
                int index = pbtn.indexOf((QPushButton*)obj);
                QKeyEvent keyPress(QEvent::KeyPress, code.at(index), Qt::NoModifier, unicode.at(index));
                QApplication::sendEvent(currentFocus, &keyPress);
            }
            break;
        }
        default:break;
    }
    return QObject::eventFilter(obj, event);
}

void KeyboardBase::keyboardCreate(QList<Qt::Key> kcode, QList<QString> kunicode, int w, int h)
{
    int number;
    number = qMin(kcode.size(), kunicode.size());

    for(int i = 0; i< number; i++)
    {
        QPushButton *pb = new QPushButton(this);    //新建按键
        pb->setText(kunicode.at(i));                //设置按符号
        pb->resize(w, h);                           //设置按键大小
        pb->setObjectName("keypushbutton");         //设置控件名称
        pb->setFocusPolicy(Qt::NoFocus);            //设置无焦点

        pbtn.append(pb);                            //保存按键
        code.append(kcode.at(i));                   //保存按键值
        unicode.append(kunicode.at(i));             //保存按键字符

        pb->installEventFilter(this);
    }
    show();
}

void KeyboardBase::keyboardLayout(const QList<QPoint> point)
{
    int number = qMin(pbtn.size(), point.size());

    for(int i = 0; i< number; i++)
    {
        pbtn.at(i)->move(point.at(i));
    }
    adjustSize();
}

void KeyboardBase::setKeySize(Qt::Key kcode, int w, int h)
{
    int index = code.indexOf(kcode);
    if(index < pbtn.size())
    {
        pbtn.at(code.indexOf(kcode))->resize(w, h);
    }
}


KbInstance::KbInstance(QWidget *parent, int w, int h)
    : KeyboardBase(parent)
{
    QList<Key> value;
    QList<QString> character;
    QList<QPoint> point;

    using namespace Qt;
    value << Key_1 << Key_2 << Key_3
          << Key_4 << Key_5 << Key_6
          << Key_7 << Key_8 << Key_9
          << Key_0 << Key_Period << Key_Backspace;
    character << "1" << "2" << "3"
              << "4" << "5" << "6"
              << "7" << "8" << "9"
              << "0" << "." << "BS";
    keyboardCreate(value, character, w, h);

    int space = 4;
    w += space;
    h += space;
    point << QPoint(0, 0) << QPoint(w, 0) << QPoint(w * 2, 0)
          << QPoint(0, h) << QPoint(w, h) << QPoint(w * 2, h)
          << QPoint(0, h * 2) << QPoint(w, h * 2) << QPoint(w * 2, h * 2)
          << QPoint(0, h * 3) << QPoint(w, h * 3) << QPoint(w * 2, h * 3) ;

    keyboardLayout(point);
}

KbInstance::~KbInstance()
{
}
