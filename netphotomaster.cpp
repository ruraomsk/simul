#include "netphotomaster.h"

NetPhotoMaster::NetPhotoMaster(DataStore *dts, QString nameSubsystem, int ip)
{
    this->dts=dts;
    this->nameSubsystem=nameSubsystem;
    this->ip=ip;
}

void NetPhotoMaster::makeStep()
{
    int idStep=(lastId>maxId)?maxId:lastId;
    for(int id=1;id<lastId;id+=idStep){
        QTcpSocket *socket=new QTcpSocket;
        socket->connectToHost(ipPhoto,static_cast<quint16>(Setup.NetPHotoPort));
        if(!socket->waitForConnected(Setup.NetPhotoTimeOut)) {
            delete socket;
            return;
        }
        QString req;
        req.sprintf("R%d %d",id,(id+idStep)>lastId?lastId:id+idStep);
        socket->write(req.toLatin1());
        QString result="";
        if(!socket->waitForReadyRead(Setup.NetPhotoTimeOut)){
            socket->close();
            delete socket;
            return;
        };
        while(1){
            QByteArray in=socket->read(128000);
            if(in.size()==0) break;
            result.append(QString::fromLatin1(in));
            if(result.contains("</vals>")) break;
            if(!socket->waitForReadyRead(Setup.NetPhotoTimeOut)) break;
        };
        if(!result.contains("</vals>"))
        {
            qInfo()<<toString()<<" bad responce "<<req;
            socket->close();
            delete socket;
            return;
        }
        QXmlStreamReader reader(result);
        while(!reader.atEnd()){
            if(reader.name().toString()=="val"  && reader.isStartElement()){
                QString value;
                int id;
                foreach (const QXmlStreamAttribute &attr, reader.attributes()) {
                    if (attr.name().toString()=="value"){
                        value=attr.value().toString();
                    }
                    if (attr.name().toString()=="id"){
                        id=attr.value().toInt();
                    }
                }
                mutex.lock();
                values.insert(id,value);
                mutex.unlock();
            }
            reader.readNext();
        }
        mutex.lock();
        lastOperation=QDateTime::currentDateTime();
        mutex.unlock();
        socket->close();
        delete socket;
        QThread::msleep(1000);
    }
}

QString NetPhotoMaster::readVariable(QString name)
{
    if(!vars.contains(name)) return "not found";
    int id=vars.value(name);
    mutex.lock();
    QString value="is array";
    if(values.contains(id)) value=values.value(id);
    mutex.unlock();
    return  value;
}

void NetPhotoMaster::run()
{
    Subsystem sub=dts->pr.subs.value(nameSubsystem);
    ipPhoto=ip==0?sub.main:sub.second;
    if(ipPhoto=="") {
        qWarning()<<"in subsystem "<<nameSubsystem<<" invalid ip";
        return;
    }
    lastId=-1;
    foreach (Variable var, sub.vars) {
        lastId=lastId<var.id?var.id:lastId;
        values.insert(var.id,"not read");
        vars.insert(var.name,var.id);
    }
    while(1){
        QThread::msleep(Setup.NetPHotoStep);
        makeStep();
    }
}

QDateTime NetPhotoMaster::getLastTime()
{
    QDateTime result;
    mutex.lock();
    result=lastOperation;
    mutex.unlock();
    return  result;
}

QString NetPhotoMaster::toString()
{
    return nameSubsystem+" "+ipPhoto;
}


