#ifndef TERMINATE_H
#define TERMINATE_H

#include <QDialog>
#include "page.h"
#include "waiting.h"
#include "prompt.h"

namespace Ui {
class Terminate;
}

class Terminate : public Page
{
    Q_OBJECT

public:
    explicit Terminate(QWidget *parent = 0);
    ~Terminate();

    void showWaiting();
    void showWaitReslut();
    void setStopReslut(QString err);

private:
    Ui::Terminate *ui;
    Waiting *waitinfo;
    Prompt  *prompt;
};

#endif // TERMINATE_H
