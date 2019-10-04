#include "routesubdevices.h"


RouteSubDevices::RouteSubDevices(DataStore *dts,QString nameSub,QObject *parent) : QObject(parent)
{
    this->dts=dts;
    this->nameSub=nameSub;
    size_t ptr=0;
    Subsystem sub=dts->pr.subs.value(nameSub);
    ips.append(sub.main);
    ips.append(sub.second);
    portInput=static_cast<quint16>(dts->pr.port+sub.id);
    portOutput=static_cast<quint16>(dts->pr.port);
    foreach (QString namedev, sub.orderDevices) {
        QString fullName=sub.name+":"+namedev;
        Device dev=dts->alldevs.value(fullName);
        ptrDevice.insert(dev.name, ptr);
        ptr+=dev.drv.lenData;
    }
    sizeBuffer=ptr;
    if(sizeBuffer!=0){
        buffer=static_cast<char *>(malloc(sizeBuffer));
        if(buffer==nullptr){
            qWarning()<<"Subsystem "<<nameSub<<" not malloc "<<sizeBuffer;
            return;
        }
    }
    tmr=new QTimer();
    tmr->setInterval(Setup.SimulStep);
    tmr->start();
    inUDP=new QUdpSocket();
    inUDP->bind(QHostAddress::Any,portInput);
    connect(inUDP, &QUdpSocket::readyRead,this,&RouteSubDevices::reciveFromSubsystem);
    connect(tmr,&QTimer::timeout,this,&RouteSubDevices::sendToSubsystem);

}



void RouteSubDevices::reciveFromSubsystem()
{
    if(buffer==nullptr||sizeBuffer==0) return;
    if(!inUDP->hasPendingDatagrams()) {
        portOutput=0;
        return;
    }
    qint64 len=inUDP->readDatagram(buffer,static_cast<qint64>(sizeBuffer),ipLastRecive,&portOutput);
    if(len!=(static_cast<qint64>(sizeBuffer))) return;
    Subsystem sub=dts->pr.subs.value(nameSub);
    foreach (QString nameDev, sub.orderDevices) {
        QString fullName=sub.name+":"+nameDev;
        Device dev=dts->alldevs.value(fullName);
        size_t ptr=ptrDevice.value(nameDev);
        dev.writeBufferDriver(buffer+ptr);
        dev.readAll();
        dts->alldevs.insert(fullName,dev);
    }
//    // Send replay
//    if(buffer==nullptr||sizeBuffer==0) return;
//    if(portOutput==0) return;
//    sub=dts->pr.subs.value(nameSub);
    foreach (QString nameDev, sub.orderDevices) {
        QString fullName=sub.name+":"+nameDev;
        Device dev=dts->alldevs.value(fullName);
        size_t ptr=ptrDevice.value(nameDev);
        dev.writeAll();
        dev.readBufferDriver(buffer+ptr);
        dts->alldevs.insert(fullName,dev);
    }
    foreach (QString ip, ips) {
        QHostAddress host=QHostAddress(ip);
        inUDP->writeDatagram(buffer,static_cast<qint64>(sizeBuffer),host,portOutput);
    }
}

void RouteSubDevices::sendToSubsystem()
{
    Subsystem sub=dts->pr.subs.value(nameSub);
    foreach (QString nameDev, sub.orderDevices) {
        QString fullName=sub.name+":"+nameDev;
        Device dev=dts->alldevs.value(fullName);
        size_t ptr=ptrDevice.value(nameDev);
        dev.writeAll();
        dev.readBufferDriver(buffer+ptr);
        dts->alldevs.insert(fullName,dev);
    }
    foreach (QString ip, ips) {
        QHostAddress host=QHostAddress(ip);
        inUDP->writeDatagram(buffer,static_cast<qint64>(sizeBuffer),host,portOutput);
    }

}
