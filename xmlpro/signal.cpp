#include "signal.h"

Signal::Signal()
{

}

Signal::Signal(const QXmlStreamReader *xmlReader)
{
    foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
        if (attr.name().toString()=="name"){
            name=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="format"){
            format=attr.value().toInt();
            continue;
        }
        if (attr.name().toString()=="mode"){
            if(attr.value().toString()=="output")mode=true;
            if(attr.value().toString()=="input") mode=false;
            continue;
        }
        if (attr.name().toString()=="address"){
            address=attr.value().toInt();
            continue;
        }
        if (attr.name().toString()=="value"){
            value=attr.value().toString();
            continue;
        }
        qWarning()<<"signal attr "+attr.name().toString();
    }


}

QString Signal::toString()
{
    return name+" "+QString::number(format)+" "+(mode?"output":"input")+" "+QString::number(address)+" "+value;
}
