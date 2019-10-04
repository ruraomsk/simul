#include "variable.h"

Variable::Variable()
{

}

Variable::Variable(const QXmlStreamReader *xmlReader)
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
        if (attr.name().toString()=="format"){
            format=attr.value().toInt();
            continue;
        }
        if (attr.name().toString()=="size"){
            size=attr.value().toInt();
            continue;
        }
        qWarning()<<"variable attr "+attr.name().toString();
    }

}

QString Variable::toString()
{
    return name+" "+description+" "+QString::number(format)+" "+QString::number(size);
}

QString Variable::valueToString()
{
    return name+"="+value;
}

void Variable::defaultValue()
{
    switch (format) {
    case 1:
        value="false";
        break;
    case 2: case 3: case 4: case 5: case 6: case 7:
        value="0";
        break;
    case 8: case 9:
        value="0.0";
        break;
    case 11: case 13:
        value="0";
        break;
    case 14: case 15:
        value="0.0";
        break;
    }
    if(size==1) return;
    QString temp=value;
    for (int i = 0; i < size; ++i) {
        temp+=" "+value;
    }
    value=temp;
}

void Variable::readValue(void *buffer, int address)
{
    void *buf=buffer;
    unsigned char *cbuf=(unsigned char *) buf;
    cbuf+=address;
    buf=(void *)cbuf;
    QString result;
    switch (format) {
    case 1:
        if(*cbuf==0) result="false";
        else result="true";
        break;
    case 2: case 3: case 4: case 5: case 6: case 7:
    {
        short *ibuf=(short *)buf;
        result= QString::number(*ibuf);
        break;
    }
    case 8: case 9:
    {
        if(isZero(buf,4)) result="0.0";
        else{
            float *fbuf=(float *)buf;
            result.sprintf("%f",*fbuf);
        }
        break;
    }
    case 11: case 13:
    {
        int *ibuf=(int *)buf;
        result= QString::number(*ibuf);
        break;
    }
    case 14: case 15:
    {
        if(isZero(buf,8)) result="0.0";
        else {
            double *fbuf=(double *)buf;
            result.sprintf("%f",*fbuf);
        }
        break;
    }
    }
    value=result;
    if(size==1) return ;
    QString temp=result;
    for (int i = 0; i < size; ++i) {
        temp+=" "+result;
    }
    value=temp;
}

void Variable::writeValue(void *buffer, int address)
{
    void *buf=buffer;
    unsigned char *cbuf=(unsigned char *) buf;
    cbuf+=address;
    buf=(void *)cbuf;
    switch (format) {
    case 1:
        if(value.compare("false")) *cbuf=0;
        else *cbuf=0xff;
        break;
    case 2: case 3: case 4: case 5: case 6: case 7:
    {
        short *ibuf=(short *)buf;
        *ibuf=(short)value.toInt();
        break;
    }
    case 8: case 9:
    {
        float *fbuf=(float *)buf;
        *fbuf=(float)value.toFloat();
        break;
    }
    case 11: case 13:
    {
        int *ibuf=(int *)buf;
        *ibuf=(int)value.toInt();
        break;
    }
    case 14: case 15:
    {
        double *fbuf=(double *)buf;
        *fbuf=value.toDouble();
        break;
    }
    }
}

bool Variable::isZero(void *buffer, int len)
{
    int count=0;
    unsigned char *cbuf=(unsigned char *)buffer;
    for (int i = 0; i < len; ++i) {
        if(*cbuf++==0) count++;
    }
    if(count==len) return true;
    return false;
}

