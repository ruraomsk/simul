#ifndef SAVE_H
#define SAVE_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>
#include "logger.h"


class Save
{
public:
    Save(QXmlStreamReader *xmlReader);
    QString toString();

    QString value;
    QString name;

};

#endif // SAVE_H
