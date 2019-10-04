#ifndef PROJECT_H
#define PROJECT_H

#include <QXmlStreamReader>
#include <QFile>
#include <QMap>
#include "subsystem.h"
#include "driver.h"
#include "logger.h"

class  Project
{

public:
    Project();
    Project(QString filepath,QString filename);

    QString toString();
    QString getError();

public:
    QString name;
    QString description;
    QString path;
    QString defdrv;
    QString simul;
    QString ip;
    int     port;

    QMap<QString,Subsystem> subs;
    QMap<QString,Driver> drivers;
private:
    QString error="no error";
    void loadDrivers(QString path);

};


#endif // PROJECT_H
