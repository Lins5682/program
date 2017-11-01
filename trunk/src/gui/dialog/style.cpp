#include "style.h"

QString Style::rgbcolor[] = {"#104e8b", "#00b14c", "#ea5514", "#595757", "#dcdddd"};

Style::Style()
{
}

void Style::setLabelText(QLabel *label, const char *sourceText, int charasize, Color color)
{
    //设置文字颜色、字体
    setLabelStyle(label, charasize, color);
    //设置文字
    label->setText(QString::fromUtf8(sourceText));
}

void Style::setLabelStyle(QLabel *label, int charasize, Color color, QFont::Weight weight)
{
    //设置文字颜色
    QPalette palette = label->palette();
    getPalette(palette, QPalette::WindowText, color);
    label->setPalette(palette);

    //设置字体
    QFont font = label->font();
    getFont(font, weight, charasize);
    label->setFont(font);
}

void Style::setLineEditStyle(QLineEdit *lineEdit, int charasize, Color tcolor, Color bcolor)
{
    QPalette palette = lineEdit->palette();

    //设置背景色
    getPalette(palette, QPalette::Base, bcolor);
    lineEdit->setPalette(palette);

    //设置文字颜色
    getPalette(palette, QPalette::Text, tcolor);
    lineEdit->setPalette(palette);

    //设置字体
    QFont font = lineEdit->font();
    getFont(font, QFont::Normal, charasize);
    lineEdit->setFont(font);

    //设置中间对齐
    lineEdit->setAlignment(Qt::AlignCenter);
}

void Style::getPalette(QPalette &palette, QPalette::ColorRole acr, Color color)
{
    QColor qcolor;
    color > white ? qcolor = rgbcolor[0] : qcolor = rgbcolor[color];
    palette.setColor(acr, qcolor);
}

void Style::getFont(QFont &font, QFont::Weight weigth, int charasize)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    font.setFamily(QString::fromUtf8("微软雅黑"));       //设置雅黑字体
    font.setPixelSize(charasize);                       //设置字体大小
    font.setWeight(weigth);                             //是否加粗
}

void Style::setButtonStyle(QPushButton *pbtn)
{
    QString styleSheet;

    pbtn->setFixedSize(QPixmap(":/pic/image/button_default.png").size());

    styleSheet = "\
            QPushButton {\
            font: normal 25pt \"微软雅黑\";\
            color: #104e86;\
            outline: none;\
            border-image: url(:/pic/image/button_default.png); \
        } \
            QPushButton:pressed {\
            font: normal 25pt \"微软雅黑\";\
            color:rgb(255,255,255);\
            outline: none;\
            border-image: url(:/pic/image/button_press.png);\
        } ";

    pbtn->setStyleSheet(styleSheet);
}

void Style::setButtonStyle(QPushButton *pbtn, QString path, QString path_pressed)
{
    QString styleSheet;

    pbtn->setFixedSize(QPixmap(path).size());

    styleSheet = "\
            QPushButton {\
            font: normal 25pt \"微软雅黑\";\
            color: #104e86;\
            outline: none;\
            border-image: url(path1); \
        } \
            QPushButton:pressed {\
            font: normal 25pt \"微软雅黑\";\
            color:rgb(255,255,255);\
            outline: none;\
            border-image: url(path2);\
        } ";

    styleSheet.replace("path1", path);
    if(path_pressed == "")
    {
        path_pressed = path;
    }
    styleSheet.replace("path2", path_pressed);

    pbtn->setStyleSheet(styleSheet);
}
