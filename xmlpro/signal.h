#ifndef SIGNAL_H
#define SIGNAL_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>
#include "logger.h"

class Signal
{
public:
    Signal();
    Signal(const QXmlStreamReader *xmlReader);
    QString toString();
    QString name;
    int format;
    bool mode;
    int address;
    QString value;
};

#endif // SIGNAL_H
