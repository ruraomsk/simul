#ifndef DEF_H
#define DEF_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>
#include "logger.h"

class Def
{
public:
    Def(QXmlStreamReader *xmlReader);
    QString toString();

    QString name;
    QString dname;
};
#endif // DEF_H
