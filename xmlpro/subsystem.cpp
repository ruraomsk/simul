#include "subsystem.h"

Subsystem::Subsystem()
{

}

Subsystem::Subsystem(const QXmlStreamReader *xmlReader)
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
        if (attr.name().toString()=="path"){
            path=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="scheme"){
            scheme=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="id"){
            id=attr.value().toInt();
            continue;
        }
        if (attr.name().toString()=="main"){
            main=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="second"){
            second=attr.value().toString();
            continue;
        }
        if (attr.name().toString()=="file"){
            file=attr.value().toString();
            if(!file.contains(".xml"))file+=".xml";
            continue;
        }
        if (attr.name().toString()=="step"){
            step=attr.value().toInt();
            continue;
        }
        if (attr.name().toString()=="result"){
            result=attr.value().toString();
            continue;
        }
        qWarning()<<"sub attr "+attr.name().toString();
    }

}

QString Subsystem::toString()
{
    QString res=name+" "+description+"  "+scheme+" "+QString::number(id)+" "
            +file+" "+main+" "+second+" "+path+" "+result+" "
            +QString::number(step)+"\n";
    res+="Variables\n";
    foreach (Variable var, vars) {
        res+="\t"+var.toString()+"\n";
    }
    res+="Devices\n";
    foreach (Device dev , devices) {
        res+="\t"+dev.toString()+"\n";
    }
    res+="Saves to file "+saveFile+"\n";
    foreach (Save save , saves) {
        res+="\t"+save.toString()+"\n";
    }
    res+="ModBuses\n";
    foreach (MModbus mod , modbuses) {
        res+="\t"+mod.toString()+"\n";
    }
    return  res;
}

bool Subsystem::loadVariables(QString filexml)
{
    int idVar=1;
    QFile file(filexml);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        error=file.fileName()+" error on open";
        qWarning()<<error;
        return false;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while (!xmlReader.atEnd()){
        if(xmlReader.name().toString()=="var"){
            Variable var=Variable(&xmlReader);
            if(var.name!=""){
                var.id=idVar++;
                vars.insert(var.name,var);
            }
        }
        xmlReader.readNext();
    }
    maxIdVariable=--idVar;
    file.close();
    return true;

}

bool Subsystem::loadDevices(QString path,QString filexml)
{
    QFile file(filexml);
    QFile assfile;
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        error=file.fileName()+" error on open";
        qWarning()<<error;
        return false;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while (!xmlReader.atEnd()){
        if(xmlReader.name().toString()=="devices"  && xmlReader.isStartElement()){
            foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                if (attr.name().toString()=="xml"){
                    QString xmlfile=attr.value().toString();
                    if(!xmlfile.contains(".xml")) xmlfile+=".xml";
                    assfile.setFileName(path+"/"+xmlfile);
                    if(!assfile.open(QFile::ReadOnly|QFile::Text)){
                        error=assfile.fileName()+" error on open";
                        qWarning()<<error;
                        return false;
                    }
                }
            }
            xmlReader.readNext();
            while(xmlReader.name().toString()!="devices"){
                if(xmlReader.name().toString()=="device"  && xmlReader.isStartElement()){
                    Device dev=Device(&xmlReader,&assfile);
                    if(dev.name!=""){
                        orderDevices.append(dev.name);
                        devices.insert(dev.name,dev);
                    }

                }
                xmlReader.readNext();
            }
        }
        xmlReader.readNext();
    }
    file.close();
    assfile.close();
    return true;
}

bool Subsystem::loadSaves(QString filexml)
{
    QFile file(filexml);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        error=file.fileName()+" error on open";
        qWarning()<<error;
        return false;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while (!xmlReader.atEnd()){
        if(xmlReader.name().toString()=="saves"  && xmlReader.isStartElement()){
            foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                if (attr.name().toString()=="name"){
                    saveFile=attr.value().toString();
                }
            }
            xmlReader.readNext();
            while(xmlReader.name().toString()!="saves"){
                if(xmlReader.name().toString()=="save"  && xmlReader.isStartElement()){
                    Save save=Save(&xmlReader);
                    if(save.name!=""){
                        saves.insert(save.name,save);
                    }
                }
                xmlReader.readNext();
            }
        }
        xmlReader.readNext();
    }
    file.close();
    return true;
}

void Subsystem::loadSubsystem(QString path, QString namefile)
{
    QFile file(path+"/"+namefile);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        error=file.fileName()+" error on open";
        qWarning()<<error;
        return;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while (!xmlReader.atEnd()){
        if (xmlReader.name().toString()=="subsystem"  && xmlReader.isStartElement()) {
            xmlReader.readNext();
            while (xmlReader.name().toString()!="subsystem") {
                if(xmlReader.name().toString()=="variable"  && xmlReader.isStartElement()){
                    foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                        if (attr.name().toString()=="xml"){
                            QString xmlfile=attr.value().toString();
                            if(!xmlfile.contains(".xml")) xmlfile+=".xml";
                            if (!loadVariables(path+"/"+xmlfile)){
                                qWarning()<<"Error load variables "<<path+"/"+xmlfile;
                            }
                        }
                    }
                }
                if(xmlReader.name().toString()=="devices"  && xmlReader.isStartElement()){
                    foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                        if (attr.name().toString()=="xml"){
                            QString xmlfile=attr.value().toString();
                            if(!xmlfile.contains(".xml")) xmlfile+=".xml";
                            if (!loadDevices(path,path+"/"+xmlfile)){
                                qWarning()<<"Error load devices "<<path+"/"+xmlfile;
                            }
                        }
                    }
                }
                if(xmlReader.name().toString()=="saves"  && xmlReader.isStartElement()){
                    foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                        if (attr.name().toString()=="xml"){
                            QString xmlfile=attr.value().toString();
                            if(!xmlfile.contains(".xml")) xmlfile+=".xml";
                            if (!loadSaves(path+"/"+xmlfile)){
                                qWarning()<<"Error load saves "<<path+"/"+xmlfile;
                            }
                        }
                    }
                }
                if(xmlReader.name().toString()=="modbus"  && xmlReader.isStartElement()){
                    MModbus mb=MModbus(&xmlReader);
                    if(mb.name!=""){
                        mb.tableModbus=TableModbus(path+"/"+mb.xml);
                        modbuses.insert(mb.name,mb);
                    }
                }
                xmlReader.readNext();
            }
        }
        xmlReader.readNext();

    }
    file.close();

}

QString Subsystem::getError()
{
    return error;
}
