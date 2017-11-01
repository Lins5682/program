#ifndef CHARGECONFIRM_H
#define CHARGECONFIRM_H

#include <QDialog>
#include "dialog/page.h"
#include "dialog/prompt.h"

namespace Ui {
class Chargeconfirm;
}

class Chargeconfirm : public Page
{
    Q_OBJECT

public:
    explicit Chargeconfirm(QWidget *parent = 0);
    ~Chargeconfirm();
    virtual void enter(void);
    virtual void exit(void);
    Prompt *prompt;
private:
    Ui::Chargeconfirm *ui;
    void initButtonStyle();

signals:
    void clickedButton(const QString &s);

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // CHARGECONFIRM_H
