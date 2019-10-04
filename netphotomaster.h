#ifndef NETPHOTOMASTER_H
#define NETPHOTOMASTER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QTcpSocket>
#include <QXmlStreamReader>
#include <QThread>
#include <QMutex>
#include "datastore.h"
#include "initparameters.h"
#define maxId 1300

extern IniParam Setup;

class NetPhotoMaster:public QThread
{
    Q_OBJECT
public:
//    NetPhotoMaster();
    NetPhotoMaster(DataStore *dts,QString nameSubsystem,int ip);
    void makeStep();
    QString readVariable(QString name);
    void run();
    QDateTime getLastTime();
    QString toString();
private:
    int lastId;
    QMap<int,QString> values;
    QMap<QString,int> vars;
    DataStore *dts;
    QString nameSubsystem;
    QString ipPhoto;
    QTimer *tmr;
    int ip;
    QMutex mutex;
    QDateTime lastOperation=QDateTime::currentDateTime();
};

#endif // NETPHOTOMASTER_H
