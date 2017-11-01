#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <QDialog>
#include <QtCore>
#include "dialog/page.h"

namespace Ui {
class PasswordInput;
}

class PasswordInput : public Page
{
    Q_OBJECT

public:
    explicit PasswordInput(QWidget *parent = 0);
    ~PasswordInput();
    void setDefaultLineEditText(const QString &s);

private:
    Ui::PasswordInput *ui;
    QSignalMapper signalMapper;
    void initButtonStyle();
    void initButtonConnect();

public:
    virtual void enter(void);
    virtual void exit(void);
    QString getPasswordLineEditText(void);
    void clearPasswordLineText();
    QWidget *getPasswordLineWidget(void);

signals:
    void sendEditSignal(QWidget *);
    void clearLineText();
    void stringGenerated(const QString &s);

    void clickedConfirmButton(QString text);
    void clickedCancleButton(void);

private slots:
    void getEditFocus(QWidget *w);
    void buttonClicked(QWidget *w);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // PASSWORDINPUT_H
