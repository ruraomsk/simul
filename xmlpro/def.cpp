#include "def.h"

Def::Def(QXmlStreamReader *xmlReader)
{
    foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
        if (attr.name().toString()=="name"){
            name=attr.value().toString();
            continue;
        }
        qWarning()<<"def attr "+attr.name().toString();
    }
    xmlReader->readNext();
    dname=xmlReader->text().toString();
}

QString Def::toString()
{
    return name+" "+dname;
}
