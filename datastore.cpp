#include "datastore.h"


DataStore::DataStore(Project *prj)
{
    pr=*prj;
    allvars.clear();
    alldevs.clear();
    foreach (Subsystem sub, pr.subs) {
        foreach (Device dev,sub.devices){
            Driver driver=pr.drivers[dev.driver];
            dev.setDriver(&allvars,&driver);
            QString subdev=sub.name+":"+dev.name;
            alldevs.insert(subdev,dev);
            //            qInfo()<<subdev;
        }
    }
//    qInfo()<<alldevs.keys();
    QStringList subname;
    Device dev;
    QList<QString> varNames;
    Subsystem sub;
    Variable var;
    QMap<QString,Device> tdevs;
    foreach (QString cname,alldevs.keys()){
        subname=cname.split(":");
        if(subname.count()!=2) {
            qWarning()<<cname;
            continue;
        }
        dev=alldevs[cname];
        varNames=dev.getSrcVariables();
        sub=pr.subs[subname[0]];
        foreach (QString nameVar, varNames) {
            if(allvars.contains(nameVar))   qWarning()<<nameVar<<" more src variables!";
            else{
                var=sub.vars[nameVar];
                var.defaultValue();
                dev.allvars->insert(nameVar,var);
            }
        }
        tdevs.insert(cname,dev);
    }
    alldevs=tdevs;
    tdevs.clear();
    foreach (QString cname,alldevs.keys()){
        subname=cname.split(":");
        if(subname.count()!=2) {
            qWarning()<<cname;
            continue;
        }
        dev=alldevs[cname];
        varNames=dev.getDstVariables();
        sub=pr.subs[subname[0]];
        foreach (QString nameVar, varNames) {
            if(allvars.contains(nameVar))   continue;
            var=sub.vars[nameVar];
            var.defaultValue();
            dev.allvars->insert(nameVar,var);
        }
        tdevs.insert(cname,dev);
    }
    alldevs=tdevs;
}

QString DataStore::toString()
{
    QString res="DataStore for "+pr.name+" "+pr.description+"\nDevices:\n";
    int count=0;
    foreach (QString name, alldevs.keys()) {
        res+=name+" ";
        if(++count%5==0) res+="\n";
    }
    res+="\nVariables:\n";
    count=0;
    foreach (QString name, allvars.keys()) {
        res+=name+" ";
        if(++count%10==0) res+="\n";
    }
    res+="\n";
    return res;
}

void DataStore::readAllDevices()
{
    qInfo()<<"readAllDevices "<<"started";

    foreach (Device dev, alldevs) {
        if(dev.srcVars.count()==0) continue;
        qInfo()<<"readAll for "<<dev.name;
        dev.readAll();
    }
    qInfo()<<"readAllDevices "<<"finished";

}

void DataStore::writeAllDevices()
{
    qInfo()<<"writeAllDevices "<<"started";
    foreach (Device dev, alldevs) {
        if(dev.dstVars.count()==0) continue;
        qInfo()<<"writeAll for "<<dev.name;
        dev.writeAll();
    }
    qInfo()<<"writeAllDevices "<<"finished";

}
