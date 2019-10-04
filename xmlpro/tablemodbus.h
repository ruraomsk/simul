#ifndef TABLEMODBUS_H
#define TABLEMODBUS_H

#include <QString>
#include <QMap>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QFile>

#include "register.h"

class TableModbus
{
public:
    TableModbus();
    TableModbus(QString file);
    QMap<QString,Register> registers;
};

#endif // TABLEMODBUS_H
