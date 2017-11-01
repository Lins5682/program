#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include <QLabel>
#include "dialog/page.h"

namespace Ui {
class Home;
}

class Home : public QDialog
{
    Q_OBJECT
    
public:

    explicit Home(QWidget *parent = 0);
    ~Home();
    QString getLineEditText(void);
    QString getPhonenumber();

private:
    void setButtonActiveStyle(QPushButton *b);
    void setButtonPassiveStyle(QPushButton *b);

    Ui::Home *ui;
    QLabel *label_log;
    QTimer *timer;

signals:
    //void sendEditSignal(QWidget *w);
    void buttonClickedSingal(int);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void timeout();
    //void getEditFocus(QWidget *w);
};

#endif // HOME_H
