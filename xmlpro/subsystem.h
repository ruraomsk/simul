#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H
#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QFile>

#include "variable.h"
#include "mmodbus.h"
#include "device.h"
#include "save.h"
#include "logger.h"

class Subsystem
{
public:
    Subsystem();
    Subsystem(const QXmlStreamReader *xmlReader);
    void loadSubsystem(QString path,QString namefile);
    QString getError();
    QString toString();

    QString name;
    QString description;
    QString scheme;
    int id;
    QString file;
    QString main;
    QString second;
    QString path;
    QString result;
    int step;
    QString saveFile;
    QMap <QString,MModbus> modbuses;
    QMap <QString,Variable> vars;
    QMap <QString,Device> devices;
    QMap <QString,Save>   saves;
    QDateTime lastOperation=QDateTime::currentDateTime();
    QList <QString> orderDevices;
    int maxIdVariable=0;
private:
    QString error="";
    bool loadVariables(QString filexml);
    bool loadDevices(QString path,QString filexml);
    bool loadSaves(QString filexml);

};

#endif // SUBSYSTEM_H
