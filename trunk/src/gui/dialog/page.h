#ifndef PAGE_H
#define PAGE_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include "style.h"

#define DELETE(p) if(p) { delete p; p = NULL; }

class Page : public QDialog
{
    Q_OBJECT

public:
    explicit Page(QWidget *parent);
    virtual ~Page();

    virtual void enter(void)
    {
        show();
    }

    virtual void exit(void)
    {
        hide();
    }

    void createCountdown(int sec);

protected:
    void createLabelTitle();
    void showLabelTitle(const char *title, const char *remind = "");
    void showLabelTitle();
    void hideLabelTitle();

    QLabel *label_img;
    QLabel *label_title;
    QLabel *label_remind;
    QLabel *label_cdsec;

    //TODO: cancel the countdown function
    QTimer *countdown;
    int     countdown_sec;

private slots:
    void showCountdownSec();
};

#endif // PAGE_H
