#include "mmodbus.h"

MModbus::MModbus()
{

}

MModbus::MModbus(const QXmlStreamReader *xmlReader)
{
    foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
        if (attr.name().toString()=="name"){
            name=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="description"){
            description=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="type"){
            type=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="ip1"){
            ip1=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="ip2"){
            ip2=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="xml"){
            xml=attr.value().toString();
            xml.replace("\\","/");
            if(!xml.contains(".xml")) xml+=".xml";
            continue;
        }
        if (attr.name().toString()=="port"){
            port=attr.value().toInt();
            continue;
        }
        if (attr.name().toString()=="step"){
            step=attr.value().toInt();
            continue;
        }
        qWarning()<<"modbus attr "+attr.name().toString();
    }
    tableModbus=TableModbus();

}

QString MModbus::toString()
{
    QString res= name+" "+description+" "+type+" ";
    if(type=="master"){
        res+=ip1+" "+ip2+" ";
    }
    res+=QString::number(port)+" "+QString::number(step)+" "+xml;
    return  res;
}
