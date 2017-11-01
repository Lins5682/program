#include "tcpthread.h"

void TCPClient::popFromBuffer()
{
    QByteArray dw;
    while(1)
    {
        mutex.lock();
        while(wbufferList.size() > 0)
        {
            dw.clear();
            dw.append(wbufferList.first());
            tcpSocket.write(dw);
            if(tcpSocket.waitForBytesWritten())
            {
                wbufferList.removeFirst();
            }
        }
        mutex.unlock();

        QEventLoop loop;
        QTimer::singleShot(1, &loop, SLOT(quit()));
        loop.exec();
    }
}

TCPThread::TCPThread(TCPClient *tcpclient)
{
    tcpClientThread = new QThread();
    tcpclient->moveToThread(tcpClientThread);
    connect(tcpClientThread, SIGNAL(started()), tcpclient, SLOT(popFromBuffer()));
    tcpClientThread->start();
}

TCPThread::~TCPThread()
{
    if(tcpClientThread->isRunning())
    {
        tcpClientThread->quit();
        tcpClientThread->wait();
    }
    delete tcpClientThread;
}
