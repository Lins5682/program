#ifndef QYDIALOGGROUP_H
#define QYDIALOGGROUP_H


#include <QtGui>
#include "qyDialogBase.h"
#include "User.h"
#include "TerminalCtrl.h"


class qyDialogBase;
class qyDialogGroup : public QObject
{
    Q_OBJECT

public :
    enum SubDialogType
    {
        eMain,
        eLogin,
        ePasswdAuth,
        eWarn,
        eReady,
        eLaunch,
        eRunning,
        eAccoutClose,
        eReplaceHint
    };

    template <class T>
    T* createDialog(const T* const null, QWidget* parent)
    {
        T* w;
        w= new T(this, parent);
        w->initialize();
        return w;
    }

    template <class T>
    T* createDialog(const T* const null, QWidget* parent,
                    const QString& titleText, const QString& titleHint)
    {
        T* w;
        w= new T(this, parent, titleText, titleHint);
        w->initialize();
        return w;
    }

    qyDialogBase* dialog(int idx)
    {
        return dialogQueue.value(idx);
    }

    explicit qyDialogGroup(const QString&, QWidget*);
    ~qyDialogGroup();

    void createDialogs(QSize);

    const QString name() { return terminalName;}

    void showEx();
    void hideEx();
    void showEx(int);
    void hideEx(int);

public slots:
    void showExAlrm(int, const QString&);
    void showExWithTerminal(int);
    void refresh();

signals:
    void passwordCompleted(const QString&);

public:
    int         counter;        // for test

private:
    QTimer      update;
    QString     terminalName;
    User*       user;           // 用户指针
    int         topDialog;      // 顶部窗口
    bool        visible;        // 应该显示
    QWidget*    parent;         // 所有组员窗口的父亲指针
    QList<qyDialogBase*> dialogQueue;   // 子窗口集合

};
#endif // QYDIALOGGROUP_H
