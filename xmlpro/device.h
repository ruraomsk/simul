#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QMutex>
#include "def.h"
#include "variable.h"
#include "driver.h"
#include "logger.h"

class  Device
{
public:
    Device();
    Device(const QXmlStreamReader *xmlReader,QIODevice *file);
    QList<QString> getSrcVariables();       //Возвращает имена переменных которые источники на данном устройстве
    QList<QString> getDstVariables();       //Возвращает имена переменных которые приемники на данном устройстве

    void setDriver(QMap<QString,Variable> *allvars,Driver *drv);
//    void setAllVariables(QMap<QString,Variable> *allvars);
    void readAll();                         // прочитать буфер во временные переменные
    void writeAll();                        // записать в буфер значения внутренних переменных

    void writeBufferDriver(void *extbuff);  // перенос из внешнего буфера во внутрений
    void readBufferDriver (void *extbuff);  // перенос из внутреннего буфера во внешний
    bool compareBufferDriver(void *extbuff); // true if equal

    QString toString();
    QList<QString> srcVars;       //имена переменных которые источники на данном устройстве
    QList<QString> dstVars;       //имена переменных которые приемники на данном устройстве

    QMap<QString,Def> defs;
    QMap<QString,Variable> *allvars;
    QMap<QString,bool> myvars;
    QString name;
    QString description;
    QString driver;
    QString slot;
    QDateTime lastOperation=QDateTime::currentDateTime();

    QMutex *mutex=new QMutex();
    Driver drv;
private:
    void *dataBuffer=nullptr;
    QMap <QString,int> nameAddress;       //Переход из имени в адрес буфера

};

#endif // DEVIICE_H
