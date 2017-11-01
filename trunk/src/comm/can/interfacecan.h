#ifndef INTERFACECAN_H
#define INTERFACECAN_H

#include "interface.h"

class InterfaceCan : public Interface
{
    Q_OBJECT
public:
    explicit InterfaceCan(const QString &name);
    ~InterfaceCan();

    virtual bool initialize();

public Q_SLOTS:
    void receiveFromRouting(QByteArray data);

private:
    int socketFD;           // socket 文件描述符
    QString interfaceName;  // interface 名称：can0 can1

private Q_SLOTS:
    void readDevice();
};

#endif // INTERFACECAN_H
