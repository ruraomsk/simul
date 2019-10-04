#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QDir>

#include "project.h"
#include "logger.h"

#include "datastore.h"
#include "simobmen.h"
#include "smbobmen.h"
#include "subobmen.h"
#include "variablefinder.h"
#include "routesubdevices.h"
#include "initparameters.h"

//#define CLI true

IniParam Setup;

USE_LOGGER
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Logger logger(QDir::homePath()+"/"+Setup.logPath);

    Project *pr=new Project(QDir::homePath()+"/"+Setup.dataPath,Setup.dataName);

    DataStore *dts=new DataStore(pr);

#ifndef CLI
    QWidget *frame=new QWidget();
    QVBoxLayout *layout=new QVBoxLayout();
    w.setCentralWidget(frame);
    frame->setLayout(layout);
    VariableFinder *varFinder=new VariableFinder();
    layout->addWidget(varFinder->frame);
    QTabWidget *tabW=new QTabWidget();
    tabW->clear();
    layout->addWidget(tabW);
    SimObmen *sim=new SimObmen(dts);
    tabW->addTab(sim->tree,"Devices");
    sim->setSize(1900,1700);
    sim->connectSlotFind(varFinder);
    sim->connectCollapsFind(varFinder);
    SubObmen *sob=new SubObmen(dts);
    tabW->addTab(sob->tree,"Variables");
    sob->setSize(1900,1700);
    sob->connectSlotFind(varFinder);
    sob->connectCollapsFind(varFinder);
    SmbObmen *smb=new SmbObmen(dts);
    tabW->addTab(smb->tree,"ModBuses");
    smb->connectSlotFind(varFinder);
    smb->connectCollapsFind(varFinder);
    smb->setSize(1900,1700);
#endif
    QMap<QString,RouteSubDevices*> routSubs;
    foreach (Subsystem sub, dts->pr.subs) {
        RouteSubDevices *rs=new RouteSubDevices(dts,sub.name);
        routSubs.insert(sub.name,rs);
    }
#ifdef CLI

#endif
#ifndef CLI

    w.resize(2000,1800);
    w.show();
#endif

    FINISH_LOG

    return a.exec();
}
