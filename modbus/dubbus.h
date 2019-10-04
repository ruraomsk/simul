#ifndef DUBBUS_H
#define DUBBUS_H

#include <QObject>
#include <QDateTime>
#include <QtDebug>
#include "mmodbus.h"
#include "subsystem.h"
#include "tcpmastersingle.h"

#define StepRead 300
#define ReconnectStep 5000


class DubBus
{
public:
    DubBus();
    DubBus(Subsystem *sub,MModbus modbus,QObject *parent = nullptr);
    QString getValue(QString nameValue);
    void setValue(QString nameValue,QString value);
    bool isConnected();
public:
    QString name;
    QString description;
private:
    TcpMasterSingle *getMaster();

    QList<TcpMasterSingle*> masters;
    QMap<QString,ModbusRegister> registers;
    QObject *parent;
};

#endif // DUBBUS_H
