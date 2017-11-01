#ifndef STARTCHARGE_H
#define STARTCHARGE_H

#include "page.h"
#include "waiting.h"
#include "prompt.h"

class StartCharge : public Page
{
    Q_OBJECT
public:
    explicit StartCharge(QWidget *parent = 0);
    ~StartCharge();

    void showWaiting();
    void showWaitReslut();
    void setWaitReslut(QString err);

private:
    Waiting *waitinfo;
    Prompt  *prompt;
};

#endif // STARTCHARGE_H
