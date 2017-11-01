#include "home.h"
#include "ui_home.h"
#include "dialog/mylineedit.h"
#include <QDebug>
#include <QBitmap>

Home::Home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    resize(800, 480);

    this->setWindowFlags(Qt::FramelessWindowHint);  //去掉标题栏

    /*蓝色背景图片*/
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/image/background01.png")));
    setPalette(palette);

    /*选枪按钮*/
    setButtonActiveStyle(ui->pushButton);
    setButtonPassiveStyle(ui->pushButton_2);

    QPixmap pix(":/pic/image/tab.png");
    ui->pushButton->setFixedSize(pix.size());
    ui->pushButton->setMask(pix.mask());
    ui->pushButton_2->setFixedSize(pix.size());
    ui->pushButton_2->setMask(pix.mask());

    /*左上角logo*/
    label_log = new QLabel(this);
    label_log->setPixmap(QPixmap(":/pic/image/insiderlogo.png"));
    label_log->setGeometry(QRect(25, 13, 180, 39));

    /*底部显示区域*/
    Style style;
    style.setButtonStyle(ui->pushButton_net, ":/pic/image/net.png");
    style.setButtonStyle(ui->pushButton_waring, ":/pic/image/swaring.png");
    style.setButtonStyle(ui->pushButton_help, ":/pic/image/help.png");

    ui->label_phoneimg->setPixmap(QPixmap(":/pic/image/phone.png"));
    style.setLabelStyle(ui->label_phonenumber, 14, Style::white);
    ui->label_phonenumber->setText(getPhonenumber());

    style.setLabelStyle(ui->label_time, 14, Style::white);

    /*定时器：刷新显示系统时间label*/
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer,SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start();
}

Home::~Home()
{
    delete ui;
}

void Home::timeout()
{
    QString date;
    date = QDateTime::currentDateTime().toString("hh:mm:ss  dddd  yyyy-MM-dd");

    date.replace("Monday", QString::fromUtf8("星期一"));
    date.replace("Tuesday", QString::fromUtf8("星期二"));
    date.replace("Wednesday", QString::fromUtf8("星期三"));
    date.replace("Thursday", QString::fromUtf8("星期四"));
    date.replace("Friday", QString::fromUtf8("星期五"));
    date.replace("Saturday", QString::fromUtf8("星期六"));
    date.replace("Sunday", QString::fromUtf8("星期日"));

    ui->label_time->setText(date);
}

QString Home::getPhonenumber()
{
    QString phonenumber;
    phonenumber = "4000000000";
    phonenumber.insert(4, "-");
    phonenumber.insert(8, "-");
    return phonenumber;
}

void Home::setButtonActiveStyle(QPushButton *b)
{
    QString styleSheet;
    styleSheet = "\
            QPushButton { border-radius: 1;\
            font: Bold 30pt \"微软雅黑\";\
            color: #104e8b;\
            outline: none;\
            border-image: url(:/pic/image/tab.png);\
        } ";
    b->setStyleSheet(styleSheet);
    b->raise();
}

void Home::setButtonPassiveStyle(QPushButton *b)
{
    QString styleSheet;
    styleSheet = "\
            QPushButton { border-radius: 1;\
            font: Bold 30pt \"微软雅黑\";\
            color:rgb(255,255,255);\
            outline: none;\
            border-image:transparent;\
        } ";
    b->setStyleSheet(styleSheet);
}

void Home::on_pushButton_clicked()
{
    emit buttonClickedSingal(1);
    setButtonActiveStyle(ui->pushButton);
    setButtonPassiveStyle(ui->pushButton_2);
}

void Home::on_pushButton_2_clicked()
{
    emit buttonClickedSingal(2);
    setButtonActiveStyle(ui->pushButton_2);
    setButtonPassiveStyle(ui->pushButton);
}
