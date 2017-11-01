#ifndef WELCOME_H
#define WELCOME_H

#include <QDialog>
#include <QString>
#include <QtGui/QLabel>
#include <QTextCodec>
#include <QtGui>
#include "dialog/page.h"

namespace Ui {
class Welcome;
}

class Welcome : public Page
{
    Q_OBJECT
    
public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();
    
private:
    Ui::Welcome *ui;

};

#endif // WELCOME_H





