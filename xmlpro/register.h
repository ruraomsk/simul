#ifndef REGISTER_H
#define REGISTER_H

#include <QObject>
#include <QXmlStreamReader>

class Register
{
public:
    Register();
    Register(QXmlStreamReader *xmlReader);

    uint lastAddress();
    uint getSizeElent();
    uint getFullSize();

    QString name;
    QString description;
    int type=0;
    int format=2;
    uint address=0;
    uint size=1;
    int unitId=1;

};

#endif // REGISTER_H
