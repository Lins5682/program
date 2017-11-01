#include "qyDialogBase.h"

QSize
qyDialogBase :: fixedSize = QSize(0, 0);

qyDialogBase :: qyDialogBase(qyDialogGroup* g, QWidget *parent)
              : QDialog(parent)
{
    isTitled = false;
    group    = g;
}

qyDialogBase :: qyDialogBase(qyDialogGroup* g, QWidget *parent, const QString& text, const QString& hint)
              : QDialog(parent)
{
    tText    = text;
    tHint    = hint;
    isTitled = true;
    group    = g;
}

void
qyDialogBase :: createTitle()
{
    titleTips = new QLabel(this);
    titleText = new QLabel(this);
    titleHint = new QLabel(this);

    titleTips->setObjectName("titleTips");
    titleText->setObjectName("titleText");
    titleHint->setObjectName("titleHint");

    titleTips->setGeometry( 30, 35,  10, 30);
    titleText->setGeometry( 50, 35, 150, 30);
    titleHint->setGeometry(200, 38, 300, 30);

    titleHint->setText(tHint);
    titleText->setText(tText);
}

void
qyDialogBase :: initialize()
{
    setWindowFlags(Qt::FramelessWindowHint);
    // TODO:
    setGeometry( 10,
                 45,
                 780/*fixedSize.width()*/,
                 400/*fixedSize.height()*/);

    //for Test
    next = new QPushButton("N", this);
    prev = new QPushButton("P", this);

    next->setGeometry(680, 360,  20,  20);
    prev->setGeometry(720, 360,  20,  20);

    next->raise();
    prev->raise();
    //
    connect(next, SIGNAL(clicked()), this, SLOT(showNextDialog()));
    connect(prev, SIGNAL(clicked()), this, SLOT(showPrevDialog()));

    if(isTitled)
        createTitle();

    initializeDialog();

    hide();
}

qyDialogGroup*
qyDialogBase :: owned()
{
    return group;
}

// for Test
void
qyDialogBase :: showNextDialog()
{
    group->showEx((group->counter = group->counter) >= 8 ? 0 : ++group->counter);
}
void
qyDialogBase::showPrevDialog()
{
    group->showEx((group->counter = group->counter) <= 0 ? 8 : --group->counter);
}

QSize
qyDialogBase :: getFixedSize()
{
    return fixedSize;
}

void
qyDialogBase :: setFixedSize(const QSize& size)
{
    fixedSize = size;
}
