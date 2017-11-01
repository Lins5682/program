#include "qyMainWindow.h"

qyMainWindow::qyMainWindow(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog
               /*| Qt::FramelessWindowHint*/
                 | Qt::WindowStaysOnTopHint);   // 对话框、无边框、置顶

    setGeometry(0,
                0,
                QApplication::desktop()->width() ,  // 显示屏居中显示
                QApplication::desktop()->height());

    tabBar      = 0;
    positive    = 0;
    network     = new QPushButton(this);
    help        = new QPushButton(this);
    warning     = new QPushButton(this);
    companyLogo = new QLabel(this);
    phoneIcon   = new QLabel(this);
    phoneText   = new QLabel(this);
    dateTimeText= new QLabel(this);

    dateTimeText->setObjectName("dataTimeText");
    dateTimeText->setGeometry(570, 450,220, 20);
    phoneText->setObjectName("phoneText");
    phoneText->setGeometry(412, 450, 120, 20);
    phoneText->setText("0000-0000-00");
    companyLogo->setObjectName("companyLogo");
    companyLogo->setGeometry(20, 5, 180, 39);
    network->setObjectName("network");
    network->setGeometry(28, 450, 18, 18);
    help->setObjectName("help");
    help->setGeometry(63, 450, 18, 18);
    warning->setObjectName("warning");
    warning->setGeometry(98, 450, 18, 18);
    phoneIcon->setObjectName("phoneIcon");
    phoneIcon->setGeometry(380, 450, 17, 19);

    dateTimeUpdating.setInterval(1000);
    dateTimeUpdating.start();
    QObject::connect(&dateTimeUpdating, SIGNAL(timeout()), this, SLOT(updateDateTime()));
}

qyMainWindow::~qyMainWindow()
{
}

void qyMainWindow::updateDateTime()
{
    /*TODO: Take out the flag of 'Week'!*/
    dateTimeText->setText(QDateTime::currentDateTime().toString("hh:mm:ss"
                                                                "dddd"
                                                                "yyyy-MM-dd"));
}

/*TODO: Optimize it!*/
void qyMainWindow::initialize(const QStringList& tlNames)
{
    if(!tlNames.empty())
    {
        if(!tabBar)
        {
            tabBar = new QTabBar(this);
            tabBar->setGeometry(240, 15, 540, 50);
            tabBar->setObjectName("tabBar");
            tabBar->setDrawBase(false);
            tabBar->setLayoutDirection(Qt::RightToLeft);
        }
        else
        {
            /*
             * TODO:
             * Add new terminals dynamically, we need to do something.
             * Such as clear up the tab items.
            */
        }
        /*
         * Note:
         * The sequence of QTabBar's serial number is N to 0,but
         * the sequence of Group's serial number is 0 to N.
        */
        for(int i = 0; i < tlNames.size(); i++)
        {
            QString name = tlNames.at(tlNames.size() - 1 - i);
            tabBar->addTab(name);
            qyDialogGroup* group = new qyDialogGroup(name, this);
            groupQueue.append(group);
        }
        groupQueue[0]->showEx();
        tabBar->setCurrentIndex(tlNames.size() - 1);
        tabBar->raise();
        QObject::connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentQueue(int)));
    }
    else
    {
        qWarning() << "The size of window's groups is empty!";
    }
}

void qyMainWindow::changeCurrentQueue(int c)
{
    groupQueue.at(positive)->hideEx();
    positive = groupQueue.size() - 1 - c;
    groupQueue.at(positive)->showEx();
    emit currentQueueChanged(c);
    g_TerminalCtrl->setFocal(groupQueue.at(positive)->name());
}

void qyMainWindow::showHelp()
{
}

void qyMainWindow::showNetwork()
{
}

void qyMainWindow::showWarning()
{
}
