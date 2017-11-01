#include "terminate.h"
#include "ui_terminate.h"

Terminate::Terminate(QWidget *parent) :
    Page(parent),
    ui(new Ui::Terminate)
{
    ui->setupUi(this);

    /*标题img,文字,提示语*/
    showLabelTitle("停止充电");

    /*等待提示语*/
    waitinfo = NULL;
    waitinfo = new Waiting(this);
    waitinfo->setWaitText(QString::fromUtf8("正在停机，请稍候"));

    /*停机失败提示语*/
    prompt = NULL;
    prompt = new Prompt(this);

    hide();
}

Terminate::~Terminate()
{
    delete ui;
}

void Terminate::showWaiting()
{
    prompt->hide();
    waitinfo->show();
}
void Terminate::showWaitReslut()
{
    waitinfo->hide();
    prompt->show();
}

void Terminate::setStopReslut(QString err)
{
    prompt->setRemind(err);
    prompt->show();
}
