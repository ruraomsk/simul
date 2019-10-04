#include "save.h"

Save::Save(QXmlStreamReader *xmlReader)
{
    foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
        if (attr.name().toString()=="value"){
            value=attr.value().toString();
            continue;
        }
        qWarning()<<"save attr "+attr.name().toString();
    }
    xmlReader->readNext();
    name=xmlReader->text().toString();
}

QString Save::toString()
{
    return  name+"="+value;
}

