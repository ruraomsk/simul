#include "dubbus.h"


DubBus::DubBus()
{

}

DubBus::DubBus(Subsystem *sub,MModbus modbus, QObject *parent)
{
    this->parent=parent;
    if(!modbus.type.contains("slave")) {
        qWarning()<<modbus.name<<" is not slave";
        return;
    }
    foreach (Register reg, modbus.tableModbus.registers) {
        if(reg.type==0){
            reg.type=0;
        }
        ModbusRegister mreg=ModbusRegister(reg);
        registers.insert(mreg.name,mreg);
    }
    name=modbus.name;
    description=modbus.description;
    if(sub->main!=""){
        TcpMasterSingle *master=new TcpMasterSingle(sub->main,modbus.port,registers);
        masters.append(master);
    }
    if(sub->second!=""){
        TcpMasterSingle *master=new TcpMasterSingle(sub->second,modbus.port,registers);
        masters.append(master);
    }
    foreach (TcpMasterSingle *master, masters) {
        master->startTcpMaster(StepRead,ReconnectStep);
    }

}

QString DubBus::getValue(QString nameValue)
{
    TcpMasterSingle *master=getMaster();
    return master->readValue(nameValue);
}

void DubBus::setValue(QString nameValue, QString value)
{
    foreach (TcpMasterSingle *master, masters) {
        if(master==nullptr) continue;
        master->writeValue(nameValue,value);
    }
}

bool DubBus::isConnected()
{
    bool result=false;
    foreach (TcpMasterSingle *master, masters) {
        result|=master->isConnect();
    }
    return result;
}

TcpMasterSingle *DubBus::getMaster()
{
//    TcpMasterSingle *result=nullptr;
    if(masters[0]->isConnect()) return masters[0];
    return masters[1];
//    QDateTime time;
//    time.setSecsSinceEpoch(0);
//    foreach (TcpMasterSingle *master, masters) {
//        if(master->getLastTime()>time){
//            time=master->getLastTime();
//            result=master;
//        }
//    }
//    if(result==nullptr) result=masters[0];
//    return result;
}
