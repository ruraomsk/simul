#ifndef INITPARAMETERS_H
#define INITPARAMETERS_H
#include <QString>
typedef struct Ini {
    int TimeReady=10;
    int Screen_Update=1000;
    int SimulStep=100;
    int Modbus_Step=500;
    int Modbus_Reconnect=5000;
    int SimulBasePort=5555;
    int NetPHotoPort=1080;
    unsigned long NetPHotoStep=5000;
    int NetPhotoTimeOut=2000;
    QString logPath="log";
    QString dataPath="dataSimul/pr";
    QString dataName="main.xml";

} IniParam;


#endif // INITPARAMETERS_H
