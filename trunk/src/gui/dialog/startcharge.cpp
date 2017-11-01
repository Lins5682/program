#include "startcharge.h"

StartCharge::StartCharge(QWidget *parent) :
    Page(parent)
{
    /*标题img,文字,提示语*/
    showLabelTitle("启动充电");

    /*等待提示语*/
    waitinfo = NULL;
    waitinfo = new Waiting(this);
    waitinfo->setWaitText(QString::fromUtf8("正在启动充电机，请稍候"));

    /*启动充电结果提示语*/
    prompt = NULL;
    prompt = new Prompt(this);

    hide();
}

StartCharge::~StartCharge()
{
    DELETE(waitinfo);
    DELETE(prompt);
}

void StartCharge::showWaiting()
{
    prompt->hide();
    waitinfo->show();
}
void StartCharge::showWaitReslut()
{
    waitinfo->hide();
    prompt->show();
}
void StartCharge::setWaitReslut(QString err)
{
    waitinfo->hide();
    prompt->setRemind(err);
    prompt->show();
}
