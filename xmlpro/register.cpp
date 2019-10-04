#include "register.h"

Register::Register()
{

}
Register::Register(QXmlStreamReader *xmlReader)
{
    if (xmlReader->name().toString()=="record" && xmlReader->isStartElement()) {
        xmlReader->readNext();
        while(xmlReader->name().toString()!="record"){
            if (xmlReader->name().toString()=="value" && xmlReader->isStartElement()){
                foreach (const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString()=="name"){
                        QString nameattr=attr.value().toString();
                        if(nameattr.contains("name")){
                            xmlReader->readNext();
                            name=xmlReader->text().toString();
                        }
                        if(nameattr.contains("description")){
                            xmlReader->readNext();
                            description=xmlReader->text().toString();
                        }
                        if(nameattr.contains("type")){
                            xmlReader->readNext();
                            type=xmlReader->text().toInt();
                        }
                        if(nameattr.contains("format")){
                            xmlReader->readNext();
                            format=xmlReader->text().toInt();
                        }
                        if(nameattr.contains("address")){
                            xmlReader->readNext();
                            address=xmlReader->text().toUInt();
                        }
                        if(nameattr.contains("size")){
                            xmlReader->readNext();
                            size=xmlReader->text().toUInt();
                        }
                        if(nameattr.contains("unitId")){
                            xmlReader->readNext();
                            unitId=xmlReader->text().toInt();
                        }
                    }
                }
                xmlReader->readNext();
            }
            xmlReader->readNext();
        }
    }
}
uint Register::getSizeElent()
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

uint Register::lastAddress()
{
    return address+getFullSize();
}


uint Register::getFullSize()
{
    return getSizeElent()*size;
}

