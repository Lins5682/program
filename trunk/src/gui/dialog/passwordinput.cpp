#include "passwordinput.h"
#include "ui_passwordinput.h"

PasswordInput::PasswordInput(QWidget *parent) :
    Page(parent),
    ui(new Ui::PasswordInput)
{
    ui->setupUi(this);
    connect(ui->lineEdit,SIGNAL(clicked(QWidget *)),
            this,SLOT(getEditFocus(QWidget *)));
    initButtonStyle();
    initButtonConnect();
    showLabelTitle("输入密码");
    hide();
}

PasswordInput::~PasswordInput()
{
    delete ui;
}
void PasswordInput::initButtonStyle()
{
    Style style;
    style.setButtonStyle(ui->pushButton);
    style.setButtonStyle(ui->pushButton_2);

    QString styleSheet;
    styleSheet = "\
            QPushButton:pressed { border-radius: 5; \
            font: normal 25pt \"微软雅黑\";\
            color: #104e86;\
            outline: none;\
            background-image: url(:/pic/image/password.png); \
        } \
            QPushButton {border-radius: 5;\
            font: normal 25pt \"微软雅黑\";\
            color:rgb(255,255,255);\
            outline: none;\
            background-image: url(:/pic/image/password_press.png);\
        } ";
    ui->panelButton_0->setStyleSheet(styleSheet);
    ui->panelButton_1->setStyleSheet(styleSheet);
    ui->panelButton_2->setStyleSheet(styleSheet);
    ui->panelButton_3->setStyleSheet(styleSheet);
    ui->panelButton_4->setStyleSheet(styleSheet);
    ui->panelButton_5->setStyleSheet(styleSheet);
    ui->panelButton_6->setStyleSheet(styleSheet);
    ui->panelButton_7->setStyleSheet(styleSheet);
    ui->panelButton_8->setStyleSheet(styleSheet);
    ui->panelButton_9->setStyleSheet(styleSheet);
    ui->panelButton_BS->setStyleSheet(styleSheet);
    ui->panelButton_CLR->setStyleSheet(styleSheet);
}
void PasswordInput::initButtonConnect()
{
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
    signalMapper.setMapping(ui->panelButton_CLR, ui->panelButton_CLR);
    signalMapper.setMapping(ui->panelButton_BS, ui->panelButton_BS);

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
    connect(ui->panelButton_CLR, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->panelButton_BS, SIGNAL(clicked()),&signalMapper, SLOT(map()));

    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));
}
void PasswordInput::enter(void)
{
    show();
    ui->lineEdit->setFocus();

}
void PasswordInput::exit(void)
{
    hide();
}
void PasswordInput::setDefaultLineEditText( const QString &s)
{
    ui->lineEdit->setPlaceholderText(s);
}

void PasswordInput::getEditFocus(QWidget *w)
{
    if(!w)return;
    emit sendEditSignal(w);
}
QString PasswordInput::getPasswordLineEditText(void)
{
    return ui->lineEdit->realText();
}
QWidget * PasswordInput::getPasswordLineWidget(void)
{
    return ui->lineEdit;
}
void PasswordInput::clearPasswordLineText()
{
    ui->lineEdit->clearRealText();
}

void PasswordInput::on_pushButton_clicked()
{
    emit clickedCancleButton();
}

void PasswordInput::on_pushButton_2_clicked()
{
    emit clickedConfirmButton(getPasswordLineEditText());

    qDebug()<<getPasswordLineEditText();
}
void PasswordInput::buttonClicked(QWidget *w)
{
    QString str = qvariant_cast<QString>(w->property(("ButtonValue")));
    emit stringGenerated(str);

    qDebug()<<str;
}
