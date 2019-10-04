#include "modbusregister.h"

ModbusRegister::ModbusRegister()
{

}

ModbusRegister::ModbusRegister(Register reg)
{
    this->name=reg.name;
    this->description=reg.description;
    this->type=reg.type;
    this->format=reg.format;
    this->address=static_cast<uint>(reg.address);
    this->size=static_cast<uint>(reg.size);
}

ModbusRegister::ModbusRegister(QString name, QString description, int type, int format, int address, int size)
{
    this->name=name;
    this->description=description;
    this->type=type;
    this->format=format;
    this->address=static_cast<uint>(address);
    this->size=static_cast<uint>(size);
}

int ModbusRegister::lastAddress()
{
    return static_cast<int>(address)+static_cast<int>(getFullSize());
}

uint ModbusRegister::getSizeElement()
{
    if(type==0||type==1){
        return 1;
    }
    switch (format) {
    case 2: case 3:
        return 1;
    case 4: case 5: case 6: case 7:
        return 2;
    case 8: case 9:
        return 2;
    case 11: case 13:
        return 4;
    case 14: case 15:
        return 4;
    }
    return 1;

}

uint ModbusRegister::getFullSize()
{
    return getSizeElement()*size;
}

QString ModbusRegister::toString()
{
    QString result=name+" "+description+" ";
    result+=QString::number(type)+" "+QString::number(format)+" "+QString::number(address)
            +" "+QString::number(size);
    return result;
}

bool ModbusRegister::writeValueFromString(QString value, modbus_mapping_t *mb_mappng)
{
    QStringList lValues=value.split(" ");
    if(lValues.count()!=static_cast<int>(size)) return false;
    int tAdr=static_cast<int>(address)-static_cast<int>(getSizeElement());
    foreach (QString val, lValues) {
        tAdr+=getSizeElement();
        if(type==0) {
            uint8_t result=0;
            if(val.toLower().contains("true")) result=1;
            if(val.toLower().contains("1")) result=1;
            mb_mappng->tab_bits[tAdr]=result;
            continue;
        }
        if(type==1) {
            uint8_t result=0;
            if(val.toLower().contains("true")) result=1;
            if(val.toLower().contains("1")) result=1;
            mb_mappng->tab_input_bits[tAdr]=result;
            continue;
        }

        if(format==2||format==3){
            // 2 bytes
            if(type==2){
                mb_mappng->tab_input_registers[tAdr]=val.toInt()&0xffff;
            } else {
                mb_mappng->tab_registers[tAdr]=val.toInt()&0xffff;

            }
        }
        if(format>=4&&format<=7){
            // 4 bytes int
            uint16_t res=static_cast<uint16_t>(val.toInt());
            if(type==2){
                mb_mappng->tab_input_registers[tAdr] = res >> 16;
                mb_mappng->tab_input_registers[tAdr+1] = res & 0xff;
            } else {
                mb_mappng->tab_registers[tAdr] = res >> 16;
                mb_mappng->tab_registers[tAdr+1] = res & 0xff;
            }
        }
        if(format>=8&&format<=9){
            // 4 bytes float
            float res=val.toFloat();
            if(type==2){
               MODBUS_SET_INT32_TO_INT16(mb_mappng->tab_input_registers,tAdr,*((int *)&res));
            } else {
                MODBUS_SET_INT32_TO_INT16(mb_mappng->tab_registers,tAdr,*((int *)&res));
            }
        }
        if(format>=11&&format<=13){
            // 8 bytes int
            long long res=val.toLongLong();
            if(type==2){
               MODBUS_SET_INT32_TO_INT16(mb_mappng->tab_input_registers,tAdr,*((long long int *)&res));
            } else {
                MODBUS_SET_INT32_TO_INT16(mb_mappng->tab_registers,tAdr,*((long long int *)&res));
            }
        }
        if(format>=14&&format<=15){
            // 8 bytes float
            double res=val.toLongLong();
            if(type==2){
               MODBUS_SET_INT32_TO_INT16(mb_mappng->tab_input_registers,tAdr,*((long long int *)&res));
            } else {
                MODBUS_SET_INT32_TO_INT16(mb_mappng->tab_registers,tAdr,*((long long int *)&res));
            }
        }
    }
    return true;
}

QString ModbusRegister::readValueToString(modbus_mapping_t *mb_mappng)
{
    QString result="";
//    if(address==273&&type==3&&format==8){
    if(name.contains("fEM_R4US80RDU")){
        result="";
    }
    uint tAdr=address;
    for (uint i=0;i<size;i++)
    {
        if(type==0) {
            if(mb_mappng->tab_bits[tAdr]) result+="true ";
            else result+="false ";
            tAdr+=getSizeElement();
            continue;
        }
        if(type==1) {
            if(mb_mappng->tab_input_bits[tAdr]) result+="true ";
            else result+="false ";
            tAdr+=getSizeElement();
            continue;
        }

        if(format==2||format==3){
            // 2 bytes
            if(type==2){
                uint16_t res=mb_mappng->tab_input_registers[tAdr];
                result+=QString::number(res);
            } else {
                uint16_t res=mb_mappng->tab_registers[tAdr];
                result+=QString::number(res);
                if(name.contains("R0MW11IP1")&&address==17){
                    name=name;
                }
            }
            result+=" ";
        }
        if(format>=4&&format<=7){
            // 4 bytes int
            if(type==2){
                int32_t res=MODBUS_GET_INT32_FROM_INT16(mb_mappng->tab_input_registers,tAdr);
                result+=QString::number(res);
            } else {
                int32_t res=MODBUS_GET_INT32_FROM_INT16(mb_mappng->tab_registers,tAdr);
                result+=QString::number(res);
            }
            result+=" ";
        }
        if(format>=8&&format<=9){
            // 4 bytes float
            if(type==2){
                uint16_t mass[2];
                mass[0]=mb_mappng->tab_input_registers[tAdr];
                mass[1]=mb_mappng->tab_input_registers[tAdr+1];
                float res=modbus_get_float_dcba(mass);
                result+=QString::number(static_cast<double>(res));
            } else {
                uint16_t mass[2];
                mass[0]=mb_mappng->tab_registers[tAdr];
                mass[1]=mb_mappng->tab_registers[tAdr+1];
                float res=modbus_get_float_dcba(mass);
                result+=QString::number(static_cast<double>(res));
            }
            result+=" ";
        }
        if(format>=11&&format<=13){
            // 8 bytes int
            if(type==2){
                long long int res=MODBUS_GET_INT64_FROM_INT16(mb_mappng->tab_input_registers,tAdr);
                result+=QString::number(res);
            } else {
                long long int res=MODBUS_GET_INT64_FROM_INT16(mb_mappng->tab_registers,tAdr);
                result+=QString::number(res);
            }
            result+=" ";
        }
        if(format>=14&&format<=15){
            // 8 bytes float
            if(type==2){
                double res=MODBUS_GET_INT64_FROM_INT16(mb_mappng->tab_input_registers,tAdr);
                result+=QString::number(res);
            } else {
                double res=MODBUS_GET_INT64_FROM_INT16(mb_mappng->tab_registers,tAdr);
                result+=QString::number(res);
            }
            result+=" ";
        }
        tAdr+=getSizeElement();
    }
    return result;

}
