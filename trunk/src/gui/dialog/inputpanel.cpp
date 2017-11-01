#include "inputpanel.h"
#include "ui_inputpanel.h"


InputPanel::InputPanel(QWidget *parent) :
    QDialog(parent,Qt::Tool | Qt::FramelessWindowHint),
    ui(new Ui::InputPanel)
{
    setWindowModality(Qt::WindowModal);

    ui->setupUi(this);
    hide();

    signalMapper.setMapping(ui->panelButton_0, ui->panelButton_0);
    signalMapper.setMapping(ui->panelButton_1, ui->panelButton_1);
    signalMapper.setMapping(ui->panelButton_2, ui->panelButton_2);
    signalMapper.setMapping(ui->panelButton_3, ui->panelButton_3);
    signalMapper.setMapping(ui->panelButton_4, ui->panelButton_4);
    signalMapper.setMapping(ui->panelButton_5, ui->panelButton_5);
    signalMapper.setMapping(ui->panelButton_6, ui->panelButton_6);
    signalMapper.setMapping(ui->panelButton_7, ui->panelButton_7);
    signalMapper.setMapping(ui->panelButton_8, ui->panelButton_8);
    signalMapper.setMapping(ui->panelButton_9, ui->panelButton_9);
    signalMapper.setMapping(ui->panelButton_Dot, ui->panelButton_Dot);
    signalMapper.setMapping(ui->panelButton_CR, ui->panelButton_CR);
    signalMapper.setMapping(ui->panelButton_BS, ui->panelButton_BS);
    signalMapper.setMapping(ui->panelButton_ESC, ui->panelButton_ESC);

    connect(ui->panelButton_0, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_1, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_2, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_3, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_4, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_5, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_6, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_7, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_8, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_9, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_Dot, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_CR, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_BS, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_ESC, SIGNAL(clicked()),&signalMapper, SLOT(map()));

    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));

}

InputPanel::~InputPanel()
{
    delete ui;
}

/*
bool InputPanel::event(QEvent *e)
{
    return QWidget::event(e);

}
*/
void InputPanel::updatePosition(const QPoint & pos)
{
    this->move(pos);
}

void InputPanel::buttonClicked(QWidget *w)
{
    QString str = qvariant_cast<QString>(w->property(("ButtonValue")));
    emit stringGenerated(str);

    qDebug()<<str;
}

