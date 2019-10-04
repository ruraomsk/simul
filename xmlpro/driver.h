#ifndef DRIVER_H
#define DRIVER_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>

#include "logger.h"
#include "signal.h"

class Driver
{
public:
    Driver();
    Driver(QXmlStreamReader *xmlReader);
    QString toString();


public:
    QString name;
    QString description;
    QString code;
    size_t lenData;
    size_t lenInit;
    QString header;
    QString type;
    QMap <QString,Signal> datas;
    QMap <QString,Signal> inits;

};

#endif // DRIVER_H
