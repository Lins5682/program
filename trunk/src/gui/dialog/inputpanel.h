#ifndef INPUTPANEL_H
#define INPUTPANEL_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class InputPanel;
}

class InputPanel : public QDialog
{
    Q_OBJECT
    
public:
    explicit InputPanel(QWidget *parent = 0);
    ~InputPanel();
    void updatePosition(const QPoint & pos);
    
private:
    Ui::InputPanel *ui;
    QSignalMapper signalMapper;
protected:
    //bool event(QEvent *e);

signals:
    void stringGenerated(const QString &s);

private slots:
    void buttonClicked(QWidget *w);

};

#endif // INPUTPANEL_H
