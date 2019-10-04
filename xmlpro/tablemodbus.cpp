#include "tablemodbus.h"

TableModbus::TableModbus()
{

}

TableModbus::TableModbus(QString namefile)
{
    QFile file(namefile);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QString error=file.fileName()+" error on open";
        qWarning()<<error;
        return;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while (!xmlReader.atEnd()){
        if (xmlReader.name().toString()=="records"  && xmlReader.isStartElement()) {
            xmlReader.readNext();
            while (xmlReader.name().toString()!="records") {
                if(xmlReader.name().toString()=="record"  && xmlReader.isStartElement()){
                    Register reg=Register(&xmlReader);
                    if(reg.name!=""){
                        registers.insert(reg.name,reg);
                    }
                }
                xmlReader.readNext();
            }
        }
        xmlReader.readNext();
    }
}
