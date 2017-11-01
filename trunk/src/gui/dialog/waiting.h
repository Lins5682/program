#ifndef WAITING_H
#define WAITING_H

#include <QLabel>
#include <QProgressBar>
#include "style.h"

class Waiting
{
public:
    explicit Waiting(QWidget *parent = 0);
    ~Waiting();

    void setWaitText(const QString remind);
    void hide();
    void show();

private:
    QWidget *widget;
    QLabel  *label_waitText;
    QProgressBar *progressbar;
    int parentwd;
    int parentht;
};

#endif // WAITING_H
