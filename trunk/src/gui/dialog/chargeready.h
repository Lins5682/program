#ifndef CHARGEREADY_H
#define CHARGEREADY_H

#include <QDialog>
#include "dialog/page.h"
#include "dialog/mylabel.h"


namespace Ui {
class Chargeready;
}


class Chargeready : public Page
{
    Q_OBJECT
public:
    explicit Chargeready(QWidget *parent = 0);
    ~Chargeready();

private:
    Ui::Chargeready *ui;

public:

    virtual void enter(void);
    virtual void exit(void);
    void setLabelVariable1(const QString &s);
    void setLabelVariable2(const QString &s);
    void setLabelVariable3(const QString &s);


signals:
    void clickedCancleButton();
    void clickedConfirmButton();
    
public slots:

    
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // CHARGEREADY_H
