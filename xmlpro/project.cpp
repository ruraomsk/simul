#include "project.h"

#include <QDebug>
#include <QDir>
#include <QFile>

Project::Project()
{

}

Project::Project(QString filepath,QString filename)
{
    QFile file(filepath+"/"+filename);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QString fatalmsg=file.fileName()+" error on open";
        qWarning()<<fatalmsg;
        error=fatalmsg;
        return;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    subs.clear();
    while (!xmlReader.atEnd()){
        //        qDebug()<<xmlReader.name().toString();
        if (xmlReader.name().toString()=="general"  && xmlReader.isStartElement()) {
            foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
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
                if (attr.name().toString()=="defdrv"){
                    defdrv=attr.value().toString();
                    continue;
                }
                if (attr.name().toString()=="simul"){
                    simul=attr.value().toString();
                    continue;
                }
                if (attr.name().toString()=="ip"){
                    ip=attr.value().toString();
                    continue;
                }
                if (attr.name().toString()=="port"){
                    port=attr.value().toInt();
                    continue;
                }
                qWarning()<<"general attr "+attr.name().toString();
            }
            xmlReader.readNext();
            continue;
        }
        if(xmlReader.name().toString()=="subs"  && xmlReader.isStartElement()){
            Subsystem sub=Subsystem(&xmlReader);
            if(sub.name!=""){
                subs.insert(sub.name,sub);
            }
            xmlReader.readNext();
            continue;

        }
        xmlReader.readNext();

    }
    file.close();
    loadDrivers(filepath+"/"+defdrv);
    foreach (Subsystem sub, subs) {
        sub.loadSubsystem(filepath+"/"+sub.path,sub.file);
        subs.insert(sub.name,sub);

    }

}
QString Project::toString()
{
    QString res=name+" "+description+" "+defdrv+" "+simul+" "+ip+" "+QString::number(port)+"\n";
    foreach (Subsystem sub, subs) {
        res+="\t"+sub.toString()+"\n";
    }
    res+="Drivers\n";
    foreach (Driver drv, drivers) {
        res+=drv.toString()+"\n";
    }
    return res;
}

QString Project::getError()
{
    return error;
}

void Project::loadDrivers(QString path)
{
    drivers.clear();
    QDir dir(path);
    foreach (QString namefile, dir.entryList(QDir::Files)) {
        QFile file(path+"/"+namefile);
        if(!file.open(QFile::ReadOnly|QFile::Text)){
            this->error=file.fileName()+" error on open";
            qWarning()<<this->error;
            return;
        }
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        Driver drv=Driver(&xmlReader);
        drivers.insert(drv.name,drv);
        file.close();
    }


}
