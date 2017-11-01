#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QEventLoop>

class TCPClient : public QObject
{
    Q_OBJECT

#define BUFFER_EVARY_MAX_SIZE    200
#define BUFFERLIST_MAX_NUMBER    50

public:
    explicit TCPClient(QString ha, quint16 p) : hostAddress(ha), port(p)
    {
        wbufferList.clear();

        connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
        connect(&tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        connect(&tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(written(qint64)));


    }
    ~TCPClient()
    {
        wbufferList.clear();
    }

    void open()
    {
        tcpSocket.connectToHost(hostAddress, port);
        if(!tcpSocket.waitForConnected(3000))
        {
            qDebug() << "connectToHost -" << tcpSocket.errorString();
        }
    }
    void close()
    {
        tcpSocket.close();
    }

private:
    bool IspushWriteBuffer(int dl)
    {
        if(!tcpSocket.isValid()) return false;

        if(dl > BUFFER_EVARY_MAX_SIZE) return false;

        if(wbufferList.size() >= BUFFERLIST_MAX_NUMBER) return false;

        return true;
    }

    bool IspushReadBuffer(int dl)
    {
        if(dl > BUFFER_EVARY_MAX_SIZE) return false;

        if(rbufferList.size() >= BUFFERLIST_MAX_NUMBER)
        {
            rbufferList.removeFirst();
        }
        return true;
    }

private:
    QString     hostAddress;
    quint16     port;
    QTcpSocket  tcpSocket;
    QMutex      mutex;

public:
    QList<QByteArray>  wbufferList;
    QList<QByteArray>  rbufferList;

signals:
    void readAll(QByteArray);

private slots:
    void error()
    {
        qDebug() << "error -" << tcpSocket.errorString();
    }
    void disconnected()
    {
        qDebug() << "disconnected -" << tcpSocket.state();
    }
    void written(qint64 bytes)
    {
        qDebug() << bytes;
    }
    void readyRead()
    {
        QByteArray dr = tcpSocket.readAll();

        mutex.lock();
        if(IspushReadBuffer(dr.size()))
        {
            rbufferList.append(dr);
        }
        mutex.unlock();

        qDebug() << "readyRead -" << dr.size() << dr.toHex() << dr;
    }

public slots:
    void pushInBuffer(QByteArray data)
    {
        mutex.lock();
        if(IspushWriteBuffer(data.size()))
        {
            wbufferList.append(data);
        }
        mutex.unlock();
    }

    void popFromBuffer();
};

class TCPThread : public QObject
{
    Q_OBJECT
public:
    explicit TCPThread(TCPClient *tcpclient);
    ~TCPThread();

public:
    QThread *tcpClientThread;
};

#endif // TCPTHREAD_H
