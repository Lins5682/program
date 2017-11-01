#ifndef QYMAINWINDOW_H
#define QYMAINWINDOW_H

#include <QtGui>
#include <QTimer>
#include <QDateTime>
#include "qyDialogGroup.h"
#include "../app/Terminal.h"

class Terminal;
class qyMainWindow : public QDialog
{
    Q_OBJECT
public:
    explicit qyMainWindow(QWidget *parent = 0);
    ~qyMainWindow();

    void initialize(const QStringList& tlNames);
    void setDialogSize(const QSize& s)
    {
        dialogSize = s;
    }
//    qyDialogGroup*  addDialogGroup();
//    void            addDialogGroup(qyDialogGroup*);
//    qyDialogGroup*  addDialogGroup(const QString&);

signals:
    void            currentQueueChanged(int);
private slots:
    void            changeCurrentQueue(int);
    void            showNetwork();
    void            showWarning();
    void            showHelp();
    void            updateDateTime();

private:
    QTimer          dateTimeUpdating;
    int             whichOne;
    QString         s;
    QSize           dialogSize;
    int             queues;
    int             positive;
    QPushButton*    help;
    QPushButton*    network;
    QPushButton*    warning;
    QLabel*         companyLogo;
    QLabel*         phoneIcon;
    QLabel*         phoneText;
    QLabel*         dateTimeText;
    QTabBar*        tabBar;
    QList<qyDialogGroup*>  groupQueue;     // 所有的中心窗口链表的队列

};

#endif // QYMAINWINDOW_H
