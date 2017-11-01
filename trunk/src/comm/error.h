#ifndef ERROR_H
#define ERROR_H

#include <QDateTime>
#include <QList>
#include <QString>
//#include <QTextStream>

// 通用错误代码
#define ERROR_INITIALIZE            -100
#define ERROR_REGSITER              -200
#define ERROR_RECEIVE               -300
#define ERROR_SEND                  -400
#define ERROR_READ                  -500
#define ERROR_WRITE                 -600
#define ERROR_SET                   -700
#define ERROR_PARAMETER             -800
#define ERROR_AFTER(e,n)            (e+n*-100) // @ N+0 N>0

// 配置
#define ERROR_CODE_SIZE_MAX         31
#define ERROR_NAME_SIZE             16

// 错误记录数据
struct ErrorData
{
    int code;
    QDateTime time;
};

// 错误记录类
class Error
{
public:
    explicit Error(const QString &name,\
                   uint sizeMaximum = ERROR_CODE_SIZE_MAX,\
                   bool handleFlag = true,\
                   bool messageFlag = true);
    ~Error();
    // 错误代码记录和显示，显示受 handle flag 控制
    void handle(bool enable);       // 显示设置
    void handleBase(int code);      // 设置错误基数
    void handle(int codeOffset);
    void handle();
    void last(uchar number = 5) const;  // code 显示不受控制

    // 获取错误数据
    const QString &name() const;    // 名称
    int code();                     // 代码
    const QDateTime &time() const;  // 时间
    void refresh();                 // 刷新为最新位置
    ulong count() const;            // 总计数

    // 消息显示，受 message flag 控制
    void message(bool enable);
    void message(const QString &text) const;
    enum Message
    {
        INITIALIZE_START,
        INITIALIZE_OK
    };
    void messageType(Message type) const;

    // 输出显示，受 output flag 控制
    static void output(bool enable);
    static void output(const QString &text, const QString &name = QString("."));
    static void outputHex(const QByteArray &data, int number = 0, const QString &name = QString("."));
    static void outputCan(const QByteArray &data, const QString &name = QString("."));

private:
    QString nameQString; // 名称 QString
    char nameString[ERROR_NAME_SIZE + 1]; // 名称 char *
    QList<ErrorData *> recode;  // 错误代码记录
    uint sizeMaximum;       // 错误代码记录最大长度
    uint position;          // 当前错误代码记录位置
    int base;               // 错误代码基数
    int codeOffset;         // 错误代码偏移
    uint codePosition;      // 数据获取代码位置
    uint timePosition;      // 数据获取时间位置
    ulong countData;        // 错误代码总计数
    bool handleFlag;        // 处理显示标志
    bool messageFlag;       // 消息显示标志
    static bool outputFlag; // 输出显示标志
    void message(const char *text) const;
};

#endif // ERRORRECODE_H
