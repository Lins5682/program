#include "mainwindow.h"
#include <iostream>
#include "observer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
}

//uint32_t MainWindow::composeid(idstruct id)
//{
//    uint32_t idtmp = 0;
//    idtmp |= id.p << 26;
//    idtmp |= (id.r = 0) << 25;
//    idtmp |= (id.dp = 0) << 24;
//    idtmp |= id.pf << 16;
//    idtmp |= id.ps << 8;
//    idtmp |= id.sa;
//    return idtmp;
//}

//void MainWindow::on_pushButton_1_clicked()
//{
//    idstruct id;
//    if(radioButton->isChecked())    //版本校验应答
//    {
//        id.p = 6;
//        id.pf = 0x08;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 2;
//        readframe.data[0] = 12;
//        readframe.data[1] = 10;
//        readframeUpdate = true;
//    }
//    if(radioButton_2->isChecked())  //充电参数确认
//    {
//        id.p = 6;
//        id.pf = 0xA0;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 1;
//        readframe.data[0] = 0;
//        readframeUpdate = true;
//    }
//    if(radioButton_3->isChecked())  //连接确认
//    {
//        id.p = 4;
//        id.pf = 0x15;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 1;
//        readframe.data[0] = 0;
//        readframeUpdate = true;
//    }
//    if(radioButton_4->isChecked())  //启动充电
//    {
//        emit signalcommand(1);
//    }
//    if(radioButton_8->isChecked())  //启动充电应答
//    {
//        id.p = 4;
//        id.pf = 0x02;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 3;
//        readframe.data[0] = 0;
//        readframe.data[1] = 0;
//        readframe.data[2] = 0;
//        readframeUpdate = true;
//    }
//    if(radioButton_5->isChecked())  //充电启动完成
//    {
//        id.p = 4;
//        id.pf = 0x11;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 3;
//        readframe.data[0] = 0;
//        readframe.data[1] = 0;
//        readframe.data[2] = 0;
//        readframeUpdate = true;
//    }
//    if(radioButton_6->isChecked())  //启动充电
//    {
//        emit signalcommand(2);
//    }
//    if(radioButton_7->isChecked())  //停止充电应答
//    {
//        id.p = 4;
//        id.pf = 0x04;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 3;
//        readframe.data[0] = 0;
//        readframe.data[1] = 0;
//        readframe.data[2] = 0;
//        readframeUpdate = true;
//    }
//    if(radioButton_9->isChecked())  //缓冲测试
//    {
//        static uint32_t data = 0;
//        id.p = 4;
//        id.pf = 0x55;
//        id.ps = 1;
//        id.sa = 2;
//        readframe.can_id = composeid(id);
//        readframe.can_dlc = 3;
//        readframe.data[0] = data++;
//        readframe.data[1] = 0;
//        readframe.data[2] = 0;
//        readframeUpdate = true;
//    }
//}
