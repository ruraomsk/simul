#ifndef MMODBUS_H
#define MMODBUS_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>

#include "logger.h"
#include "tablemodbus.h"
class MModbus
{
public:
    MModbus();
    MModbus(const QXmlStreamReader *xmlReader);
    QString toString();

    QString name;
    QString description;
    QString type;
    QString ip1="";
    QString ip2="";
    int port;
    int step;
    QString xml;
    TableModbus tableModbus;

};

#endif // MMODBUS_H
