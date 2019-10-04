#ifndef MODBUSREGISTER_H
#define MODBUSREGISTER_H

#include <QString>
#include <QStringList>
#include <modbus.h>
#include "register.h"
class ModbusRegister
{
public:
    ModbusRegister();
    ModbusRegister(Register reg);
    ModbusRegister(QString name,QString description,int type,int format,int address,int size);

    int lastAddress();
    uint getSizeElement();
    uint getFullSize();
    QString toString();
    bool writeValueFromString(QString value,modbus_mapping_t *mb_mappng);
    QString readValueToString(modbus_mapping_t *mb_mappng);

    QString name;
    QString description;
    int type=0;
    int format=2;
    uint address=0;
    uint size=1;
    int unitId=1;

};

#endif // MODBUSREGISTER_H
