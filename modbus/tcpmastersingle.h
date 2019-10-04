#ifndef TCPMASTERSINGLE_H
#define TCPMASTERSINGLE_H


#include <QString>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QDateTime>
#include <QQueue>
#include <QTimer>
#include <QDebug>

#include <modbus.h>
#include "modbusregister.h"

#define MaxRegisters 60
class Job{
public:
    Job(QString name,QString value);
    QString name;
    QString value;
};
class TcpMasterSingle: public QThread
{
public:
    TcpMasterSingle();
    TcpMasterSingle(QString ip,int port,QMap<QString,ModbusRegister> registers);
//    TcpMasterSingle(const TcpMasterSingle &master);
    bool startTcpMaster(int interval,int reconnect);
    QDateTime getLastTime();
    QString toString();
    QString dumpMapping(modbus_mapping_t *map);
    void run();
    QString readValue(QString name);
    void writeValue(QString name,QString value);
    void CheckConnect();
    bool isConnect();
private:
    QMutex mutex;
    QDateTime lastOperation;
    QByteArray ip;
    int port;
    QQueue<Job> jobs;
    QMap<QString,ModbusRegister> registers;
    int interval;
    int reconnect;
    int flowCount;
    bool Connect=false;

    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    modbus_mapping_t *dr_mapping;
    int lenCoils=-1,lenIrs=-1,lenDis=-1,lenHrs=-1;
    void moveMapping(const modbus_mapping_t *src,modbus_mapping_t *dst);
    void readAllCoils();
    void readAllDi();
    void readAllIr();
    void readAllHr();
    void stopModbus();
    void startModbus();

};

#endif // TCPMASTERSINGLE_H
