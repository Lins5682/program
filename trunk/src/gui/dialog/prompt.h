#ifndef PROMPT_H
#define PROMPT_H

#include <QLabel>
#include "style.h"

class Prompt
{
public:
    explicit Prompt(QWidget *parent = 0);
    ~Prompt();

    enum RemindImg
    {
        waring,
        err,
        ok
    };
    static QString remindimg[3];

    void setRemindImg(const RemindImg imgtype);
    void setRemindImg(const QString imgpath);
    void setRemindText(const QString remind);
    void setRemind(const QString remind, const RemindImg imgtype = err);
    void hide();
    void show();

private:
    QWidget *widget;
    QLabel *label_img;
    QLabel *label_remind;
    int parentwd;
    int parentht;

    void createRemindLabel(QWidget *parent);
};

#endif // PROMPT_H
