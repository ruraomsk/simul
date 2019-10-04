#include "tcpmastersingle.h"
TcpMasterSingle::TcpMasterSingle()
{

}

TcpMasterSingle::TcpMasterSingle(QString ip,int port,QMap<QString,ModbusRegister> registers)
{
    this->ip=ip.toLocal8Bit();
    this->port=port;
    this->registers=registers;
    lastOperation.setSecsSinceEpoch(0);
    ctx=modbus_new_tcp(this->ip.data(),port);
    if(ctx==nullptr){
        qWarning()<<ip<<":"<<port<<" not started";
        return;
    }
    foreach (ModbusRegister reg, registers) {
        switch (reg.type) {
        case 0:
            lenCoils=(reg.lastAddress()>lenCoils)?reg.lastAddress():lenCoils;break;
        case 1:
            lenDis=(reg.lastAddress()>lenDis)?reg.lastAddress():lenDis; break;
        case 2:
            lenIrs=(reg.lastAddress()>lenIrs)?reg.lastAddress():lenIrs; break;
        case 3:
            lenHrs=(reg.lastAddress()>lenHrs)?reg.lastAddress():lenHrs; break;
        }
    }
    lenCoils=lenCoils<0?0:lenCoils;
    lenDis=lenDis<0?0:lenDis;
    lenIrs=lenIrs<0?0:lenIrs;
    lenHrs=lenHrs<0?0:lenHrs;

    mb_mapping=modbus_mapping_new(lenCoils,lenDis,lenHrs,lenIrs);
    if(mb_mapping==nullptr){
        qWarning()<<ip<<":"<<port<<" bad mapping registers";
        free(ctx);
        return;
    }
    dr_mapping=modbus_mapping_new(lenCoils,lenDis,lenHrs,lenIrs);
    if(dr_mapping==nullptr){
        qWarning()<<ip<<":"<<port<<" bad driver mapping registers";
        free(mb_mapping);
        free(ctx);
        return;
    }
}


bool TcpMasterSingle::startTcpMaster(int interval, int reconnect)
{
    this->interval=interval;
    this->reconnect=reconnect;
    flowCount=reconnect;
    Connect=true;
    if(modbus_connect(ctx)==-1){
        qInfo()<<"Modbus "<<ip<<":"<<port<<" not connected";
        Connect=false;
    }
//    qInfo()<<ip<<":"<<port<<" "<<Connect<<" start section... ";
    this->start();
    return  Connect;

}

QDateTime TcpMasterSingle::getLastTime()
{
    return lastOperation;
}

QString TcpMasterSingle::toString()
{
    QString result= ip+":"+QString::number(port)+" last="+lastOperation.toString("hh:mm:ss:zzz")
                    +(Connect?" Connected":" not Connected")+"\n";
    foreach (ModbusRegister reg, registers) {
        result+=reg.toString()+"="+reg.readValueToString(dr_mapping)+"\n";
    }
    return  result;
}

QString TcpMasterSingle::dumpMapping(modbus_mapping_t *map)
{
    QString res="dump :[";
    for (int i = 0; i < map->nb_bits; i++) {
        res+=QString::number(map->tab_bits[i])+" ";
    }
    res+="] [";
    for (int i = 0; i < map->nb_input_bits; i++) {
        res+=QString::number(map->tab_input_bits[i])+" ";
    }
    res+="] [";
    for (int i = 0; i < map->nb_input_registers; i++) {
        res+=QString::number(map->tab_input_registers[i])+" ";
    }
    res+="] [";
    for (int i = 0; i < map->nb_registers; i++) {
        res+=QString::number(map->tab_registers[i])+" ";
    }
    res+="].";
    return  res;
}

void TcpMasterSingle::run()
{
    while(true){
        while(Connect){
            QDateTime start=QDateTime::currentDateTime();
            while(!jobs.isEmpty()){
                mutex.lock();
    //            moveMapping(mb_mapping,dr_mapping);
                Job job=jobs.dequeue();
                auto reg=registers.value(job.name);
                if(reg.type==1||reg.type==2) continue;

                reg.writeValueFromString(job.value,dr_mapping);
    //            qInfo()<<"!"<<dumpMapping(dr_mapping);
                moveMapping(dr_mapping,mb_mapping);
                mutex.unlock();
                if(reg.type==0){
                    if(modbus_write_bits(ctx,static_cast<int>(reg.address),static_cast<int>(reg.getFullSize()),mb_mapping->tab_bits+reg.address)<0){
                        stopModbus();
                    }
                }
                if(reg.type==3){
                    if(modbus_write_registers(ctx,static_cast<int>(reg.address),static_cast<int>(reg.getFullSize()),mb_mapping->tab_registers+reg.address)<0){
                        stopModbus();
                    }
                }
                if(!Connect) break;
            }
            readAllCoils();
            if(!Connect) {stopModbus();break;}
            readAllDi();
            if(!Connect) {stopModbus();break;}
            readAllIr();
            if(!Connect) {stopModbus();break;}
            readAllHr();
            if(!Connect) {stopModbus();break;}
            mutex.lock();
            moveMapping(mb_mapping,dr_mapping);
            mutex.unlock();
            lastOperation=QDateTime::currentDateTime();
            qint64 stepTime=start.msecsTo(lastOperation);
            if(stepTime>=interval){
                qInfo()<<ip<<":"<<port<<" long step "<<stepTime;
                continue;
            }
            stepTime=interval-stepTime;
            QThread::msleep(static_cast<quint64>(stepTime));
        }
        lastOperation.setSecsSinceEpoch(0);
        QThread::msleep(static_cast<quint64>(interval));
        flowCount-=interval;
        if(flowCount<0){
            flowCount=reconnect;
            CheckConnect();
        }
    }
}

QString TcpMasterSingle::readValue(QString name)
{
    if(!registers.contains(name)) return "not found";
    auto reg=registers.value(name);
    mutex.lock();
    QString result=reg.readValueToString(dr_mapping);
    mutex.unlock();
    return result;
}

void TcpMasterSingle::writeValue(QString name, QString value)
{
    if(!registers.contains(name)) return;
    Job job=Job(name,value);
    mutex.lock();
    jobs.append(job);
    mutex.unlock();
}


void TcpMasterSingle::CheckConnect()
{
    if(Connect) return;
    while(!jobs.isEmpty()){
        mutex.lock();
        jobs.dequeue();
        mutex.unlock();
    }
    startModbus();
}

bool TcpMasterSingle::isConnect()
{
    return Connect;
}

void TcpMasterSingle::moveMapping(const modbus_mapping_t *src, modbus_mapping_t *dst)
{
    for (int i = 0; i < src->nb_bits; i++) {
        dst->tab_bits[i] = src->tab_bits[i];
    }
    for (int i = 0; i < src->nb_input_bits; i++) {
        dst->tab_input_bits[i] = src->tab_input_bits[i];
    }
    for (int i = 0; i < src->nb_input_registers; i++) {
        dst->tab_input_registers[i] = src->tab_input_registers[i];
    }
    for (int i = 0; i < src->nb_registers; i++) {
        dst->tab_registers[i] = src->tab_registers[i];
    }

}

void TcpMasterSingle::readAllCoils()
{
    if(mb_mapping->nb_bits==0) return;
    if(modbus_read_bits(ctx,mb_mapping->start_bits,mb_mapping->nb_bits,mb_mapping->tab_bits)<0){
        Connect=false;
        qInfo()<<ip<<":"<<port<<" coils error ... "<<modbus_strerror(errno);

    }
}

void TcpMasterSingle::readAllDi()
{
    if(mb_mapping->nb_input_bits==0) return;
    if(modbus_read_input_bits(ctx,mb_mapping->start_input_bits,mb_mapping->nb_input_bits,mb_mapping->tab_input_bits)<0){
        Connect=false;
        qInfo()<<ip<<":"<<port<<" di error ... "<<modbus_strerror(errno);
    }
}

void TcpMasterSingle::readAllIr()
{
    if(mb_mapping->nb_input_registers==0) return;
    int address=0,nRegisters;
    uint16_t dst[MaxRegisters];
    while(address<mb_mapping->nb_input_registers){
        nRegisters=((mb_mapping->nb_input_registers-address)>MaxRegisters)
                    ?MaxRegisters:mb_mapping->nb_input_registers-address;
        if(modbus_read_input_registers(ctx,address,nRegisters,dst)<0){
            Connect=false;
            qInfo()<<ip<<":"<<port<<" ir error ... "<<modbus_strerror(errno);
            return;
        }
        for (int i = address; i < nRegisters; i++) {
            mb_mapping->tab_input_registers[i] = dst[i];
        }
        address+=nRegisters;
    }
}

void TcpMasterSingle::readAllHr()
{
    if(mb_mapping->nb_registers==0) return;
    int address=0,nRegisters;
    uint16_t dst[MaxRegisters];
    while(address<mb_mapping->nb_registers){
        nRegisters=((mb_mapping->nb_registers-address)>MaxRegisters)
                    ?MaxRegisters:mb_mapping->nb_registers-address;
        if(modbus_read_registers(ctx,address,nRegisters,dst)<0){
            Connect=false;
            qInfo()<<ip<<":"<<port<<" hr error ... "<<modbus_strerror(errno);

            return;
        }
        for (int i = address; i < nRegisters; i++) {
            mb_mapping->tab_registers[i] = dst[i];
        }
        address+=nRegisters;
    }
}

void TcpMasterSingle::stopModbus()
{
    mutex.lock();
    Connect=false;
    modbus_close(ctx);
    modbus_free(ctx);
    mutex.unlock();
}

void TcpMasterSingle::startModbus()
{
    mutex.lock();
    ctx=modbus_new_tcp(this->ip.data(),port);
    if(ctx==nullptr){
        qWarning()<<ip<<":"<<port<<" not started";
        mutex.unlock();
        return;
    }

    if(modbus_connect(ctx)==-1){
//        qInfo()<<"Modbus "<<ip<<":"<<port<<" not connected";
        Connect=false;
        modbus_close(ctx);
        modbus_free(ctx);
    } else Connect=true;
    mutex.unlock();
    if(Connect){
        qInfo()<<"Modbus "<<ip<<":"<<port<<" restore connect";

    }
}

Job::Job(QString name, QString value)
{
    this->name=name;
    this->value=value;
}
