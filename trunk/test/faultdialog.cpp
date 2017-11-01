#include "faultdialog.h"
#include "ui_faultdialog.h"

void FaultMode::pbtnFault(int type, bool ISFault)
{
    emit sendToExternFault(type, ISFault);
}
void FaultMode::showAddFault(int level, int type, QString msg)
{
    faultdialog->showAddFault(gunnumber, level, type, msg);
}
void FaultMode::showDeleteFault(int type)
{
    faultdialog->showDeleteFault(gunnumber, type);
}

Faultdialog::Faultdialog(int number, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Faultdialog)
{
    ui->setupUi(this);

    ui->lineEdit->setText("1");

    QStringList header;
    header << "LEVEL" << "NO." << "CODE" << "DESCRIBE";
    ui->tableWidget->setHorizontalHeaderLabels(header);     //设置行表头
    ui->tableWidget->resizeRowsToContents();                //行高自适应
    ui->tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    ui->tableWidget->setColumnWidth(0, 50);                 //设置列宽
    ui->tableWidget->setColumnWidth(1, 50);
    ui->tableWidget->setColumnWidth(2, 50);
    ui->tableWidget->setColumnWidth(3, 200);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    QFont font;
    font.setFamily("monaco");
    ui->tableWidget->setFont(font);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //不可编辑

    gunnumber = number;
    for(int i = 0; i < gunnumber; i++)
    {
        faultmode.append(new FaultMode(this, i + 1));
        fault.append(new Fault(i + 1));
        connect(faultmode.at(i), SIGNAL(sendToExternFault(int, bool)), fault.at(i), SLOT(revExternFault(int, bool)));
        connect(fault.at(i), SIGNAL(addFault(int, int, QString)), faultmode.at(i), SLOT(showAddFault(int, int, QString)));
        connect(fault.at(i), SIGNAL(deleteFault(int)), faultmode.at(i), SLOT(showDeleteFault(int)));
    }
}

Faultdialog::~Faultdialog()
{
    delete ui;
    qDeleteAll(fault);
    fault.clear();
    qDeleteAll(faultmode);
    faultmode.clear();
}

void Faultdialog::showAddFault(int gunnumber, int level, int type, QString msg)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(level)));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(gunnumber)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(type)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(msg));
}

void Faultdialog::showDeleteFault(int gunnumber, int type)
{
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        if(gunnumber == ui->tableWidget->item(i, 1)->text().toInt())
        {
            if(type == ui->tableWidget->item(i, 2)->text().toInt())
            {
                ui->tableWidget->removeRow(i);
                break;
            }
        }
    }
}

void Faultdialog::pbtnWork(bool ISFault)
{
    int gunnumber = ui->lineEdit->text().toInt();
    int type = ui->comboBox->currentIndex() + 1;
    faultmode.at(gunnumber - 1)->pbtnFault(type, ISFault);
}

void Faultdialog::on_pushButton_clicked()
{
    pbtnWork(true);
}

void Faultdialog::on_pushButton_2_clicked()
{
    pbtnWork(false);
}
