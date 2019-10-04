#include "device.h"

Device::Device()
{

}

Device::Device(const QXmlStreamReader *xmlReader,QIODevice *file)
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
        if (attr.name().toString()=="slot"){
            slot=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="driver"){
            driver=attr.value().toString();
            continue;
        }
        qWarning()<<"device attr "+attr.name().toString();
    }
    QXmlStreamReader xmlDefReader;
    file->seek(0);
    xmlDefReader.setDevice(file);
    xmlDefReader.readNext();
    while(!xmlDefReader.atEnd()){
        if(xmlDefReader.name().toString()==name && xmlDefReader.isStartElement()){
            xmlDefReader.readNext();
            while(xmlDefReader.name().toString()!=name){
                if(xmlDefReader.name().toString()=="def" && xmlDefReader.isStartElement()){
                    Def def=Def(&xmlDefReader);
                    if(def.name!="") {
                        defs.insert(def.name,def);
                    }
                }
                xmlDefReader.readNext();
            }
        }
        xmlDefReader.readNext();
    }
}


QList<QString> Device::getSrcVariables()
{
    srcVars.clear();
    foreach (Def def, defs) {
        Signal sig=drv.datas.value(def.dname);
        if (sig.mode) {
            srcVars.append(def.name);
            myvars.insert(def.name,true);
            nameAddress.insert(def.name,sig.address);
        }
    }
//    qInfo()<<nameAddress.keys();
    return srcVars;
}

QList<QString> Device::getDstVariables()
{
    dstVars.clear();
    foreach (Def def, defs) {
        Signal sig=drv.datas.value(def.dname);
        if (!sig.mode) {
            dstVars.append(def.name);
            nameAddress.insert(def.name,sig.address);
            myvars.insert(def.name,false);
        }
    }
    return dstVars;
}

void Device::setDriver(QMap<QString,Variable> *allvars,Driver *drv)
{
    this->allvars=allvars;
    this->drv=*drv;
    dataBuffer=malloc(drv->lenData);
    memset(dataBuffer,0,drv->lenData);
}

void Device::readAll()
{
    Variable var;
    int address;
    mutex->lock();
    foreach (QString nameVar, srcVars) {
        var=allvars->value(nameVar);
        address=nameAddress.value(nameVar);
        var.readValue(dataBuffer,address);
//        qInfo()<<"read var"<<var.name<<"="<<var.valueToString();
        allvars->insert(nameVar,var);
    }
    mutex->unlock();
}

void Device::writeAll()
{
    Variable var;
    int address;
    mutex->lock();
    foreach (QString nameVar, dstVars) {
        var=allvars->value(nameVar);
        address=nameAddress.value(nameVar);
        var.writeValue(dataBuffer,address);
//        qInfo()<<"write var"<<var.name<<"="<<var.valueToString();
    }
    mutex->unlock();
}
// перенос из внешнего буфера во внутрений
void Device::writeBufferDriver(void *extbuff)
{
    mutex->lock();
    lastOperation=QDateTime::currentDateTime();
    memcpy(dataBuffer,extbuff,drv.lenData);
    mutex->unlock();
}
// перенос из внутреннего буфера во внешний
void Device::readBufferDriver(void *extbuff)
{
//    lastOperation=QDateTime::currentDateTime();
    mutex->lock();
    memcpy(extbuff,dataBuffer,drv.lenData);
    mutex->unlock();
}
// true if equal
bool Device::compareBufferDriver(void *extbuff)
{
    mutex->lock();
    int res=memcmp(dataBuffer,extbuff,drv.lenData);
    mutex->unlock();
    if(res==0)return true;
    return false;
}


QString Device::toString()
{
    QString res="Device:"+name+" "+description+" "+slot+" "+driver+"\n";
    foreach (Def def, defs) {
        res+=def.toString()+"\n";
    }
    return res;
}

