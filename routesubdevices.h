#ifndef ROUTESUBDEVICES_H
#define ROUTESUBDEVICES_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include "datastore.h"
#include "initparameters.h"

extern IniParam Setup;

class RouteSubDevices: public QObject
{
    Q_OBJECT
public:
    explicit RouteSubDevices(DataStore *dts,QString nameSub,QObject *parent = nullptr);
    void reciveFromSubsystem();
    void sendToSubsystem();
    QString nameSub;

private:
    char *buffer=nullptr;
    DataStore *dts;
    QMap<QString,size_t> ptrDevice;
    size_t sizeBuffer=0;

    QUdpSocket *inUDP;

    QStringList ips;
    QHostAddress *ipLastRecive=new QHostAddress();
    quint16 portInput;
    quint16 portOutput=5555;
    QTimer *tmr;
signals:

public slots:
};

#endif // ROUTESUBDEVICES_H
