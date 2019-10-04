#include "driver.h"

Driver::Driver()
{
    datas.clear();
    inits.clear();
}

Driver::Driver(QXmlStreamReader *xmlReader)
{
    datas.clear();
    inits.clear();
    xmlReader->readNext();
    while(!xmlReader->atEnd()){
        if (xmlReader->name().toString()=="driver") {
            foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                if (attr.name().toString()=="name"){
                    name=attr.value().toString();
                    continue;
                }
                if (attr.name().toString()=="description"){
                    description=attr.value().toString();
                    continue;
                }
                if (attr.name().toString()=="code"){
                    code=attr.value().toString();
                    continue;
                }
                if (attr.name().toString()=="lenData"){
                    lenData=(size_t)attr.value().toInt();
                    continue;
                }
                if (attr.name().toString()=="lenInit"){
                    lenInit=(size_t)attr.value().toInt();
                    continue;
                }
                if (attr.name().toString()=="header"){
                    header=attr.value().toString();
                    continue;
                }
                qWarning()<<"driver attr "+attr.name().toString();
            }
            xmlReader->readNext();
            continue;
        }
        if (xmlReader->name().toString()=="signals" && xmlReader->isStartElement()) {
            xmlReader->readNext();
            while(xmlReader->name().toString()!="signals"){
                if (xmlReader->name().toString()=="signal" && xmlReader->isStartElement()){
                    Signal sig=Signal(xmlReader);
                    if(sig.name!="") datas.insert(sig.name,sig);
                }
                xmlReader->readNext();
            }
            xmlReader->readNext();
            continue;
        }
        if (xmlReader->name().toString()=="init") {
            foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                if (attr.name().toString()=="type"){
                    type=attr.value().toString();
                    continue;
                }
            }
            xmlReader->readNext();
            while(xmlReader->name().toString()!="init"){
                if (xmlReader->name().toString()=="signal" && xmlReader->isStartElement()){
                    Signal sig=Signal(xmlReader);
                    if(sig.name!="") inits.insert(sig.name,sig);
                }
                xmlReader->readNext();
            }
            xmlReader->readNext();
            continue;
        }
        xmlReader->readNext();
    }
}

QString Driver::toString()
{
    QString res="Driver "+name+" "+description+" "+code+" "+QString::number(lenData)+" "+QString::number(lenInit)+" "+header+"\n";
    foreach (Signal sig, datas) {
        res+="\t"+sig.toString()+"\n";
    }
    res+="Init="+type+"\n";
    foreach (Signal sig, inits) {
        res+="\t"+sig.toString()+"\n";
    }
    return res;
}
