#ifndef DATASTORE_H
#define DATASTORE_H
#include <QDebug>
#include <QStringList>
#include "project.h"
#include "subsystem.h"
#include "variable.h"
#include "device.h"

class DataStore
{
public:
    DataStore(Project *prj);
    QString toString();

    void readAllDevices();
    void writeAllDevices();

    QMap<QString,Variable> allvars;
    QMap<QString,Device> alldevs;
    Project pr;
};

#endif // DATASTORE_H
