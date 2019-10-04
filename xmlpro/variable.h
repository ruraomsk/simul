#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>

#include "logger.h"

class  Variable
{
public:
    Variable();
    Variable(const QXmlStreamReader *xmlReader);
    QString toString();
    QString valueToString();
    void defaultValue();

    void readValue(void *buffer,int address);
    void writeValue(void *buffer,int address);
public:
    QString name;
    QString description;
    int format;
    int size=1;
    int id=0;

    QString value="";
private:
    bool isZero(void *buffer,int len);

};

#endif // VARIABLE_H
