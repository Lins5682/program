#ifndef STYLE_H
#define STYLE_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qpalette.h>
#include <QTextCodec>

class Style
{
public:
    Style();

    enum Color
    {
        blue,
        green,
        orange,
        black,
        white
    };
    static QString rgbcolor[5];

    void setLabelText(QLabel *label, const char *sourceText, int charasize, Color color);

    void setLabelStyle(QLabel *label,                               //label
                       int charasize = 15,                          //文字大小
                       Color color = blue,                          //文字颜色
                       QFont::Weight weight = QFont::Normal);       //文字风格：加粗等

    void setLineEditStyle(QLineEdit *lineEdit,                      //lineEdit
                          int charasize = 15,                       //文字大小
                          Color tcolor = blue,                      //文字颜色
                          Color bcolor = white);                    //背景颜色

    void setButtonStyle(QPushButton *pbtn);                         //QPushButton风格

    void setButtonStyle(QPushButton *pbtn,                          //PushButton
                        QString path,                               //图片路径
                        QString path_pressed = "");                 //按下图片路径

private:
    void getPalette(QPalette &palette, QPalette::ColorRole acr, Color color);
    void getFont(QFont &font, QFont::Weight weigth, int charasize);
};

#endif // STYLE_H
