#ifndef INPUTCONTEXTBASE_H
#define INPUTCONTEXTBASE_H
#include "dialog/inputpanel.h"
#include "mi/mi.h"
class mi;
class InputContextBase : public QObject
{
    Q_OBJECT
public:
     explicit InputContextBase(mi *m)
     {
         this->m = m;
     }
     virtual ~InputContextBase(){}
     virtual void handle(const QString &s){}
     virtual void show(){}
     virtual void hide(){}
     virtual void updatePosition(const QPoint & pos){}
     mi * getmi(void)
     {
         return m;
     }
     mi *m;

};

class PasswordInputContext :public InputContextBase
{
    Q_OBJECT
public:
    explicit PasswordInputContext(mi *m):InputContextBase(m)
    {
        inputPanel = new InputPanel;
        connect(inputPanel,SIGNAL(stringGenerated(QString)),
                this,SLOT(handle(QString)));
    }
    virtual ~PasswordInputContext()
    {
        delete inputPanel;
    }
    virtual void show()
    {
        inputPanel->show();
    }
    virtual void hide()
    {
        inputPanel->hide();
    }
    virtual void updatePosition(const QPoint & pos)
    {
        inputPanel->updatePosition(pos);
    }
private:
    InputPanel    *inputPanel;
private slots:
    virtual void handle(const QString &s);

};
#endif // INPUTCONTEXTBASE_H
