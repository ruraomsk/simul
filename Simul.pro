#-------------------------------------------------
#
# Project created by QtCreator 2019-02-05T13:41:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Simul
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    simobmen.cpp \
    subobmen.cpp \
    variablefinder.cpp \
    routesubdevices.cpp \
    smbobmen.cpp \
    datastore.cpp \
    netphotomaster.cpp \
    $$PWD/modbus/modbusregister.cpp \
    $$PWD/modbus/tcpmastersingle.cpp \
    $$PWD/modbus/dubbus.cpp \
    $$PWD/xmlpro/project.cpp \
    $$PWD/xmlpro/subsystem.cpp \
    $$PWD/xmlpro/signal.cpp \
    $$PWD/xmlpro/driver.cpp \
    $$PWD/xmlpro/mmodbus.cpp \
    $$PWD/xmlpro/variable.cpp \
    $$PWD/xmlpro/device.cpp \
    $$PWD/xmlpro/def.cpp \
    $$PWD/xmlpro/save.cpp \
    $$PWD/xmlpro/logger.cpp \
    $$PWD/xmlpro/register.cpp \
    $$PWD/xmlpro/tablemodbus.cpp

HEADERS += \
    mainwindow.h \
    simobmen.h \
    subobmen.h \
    variablefinder.h \
    routesubdevices.h \
    smbobmen.h \
    datastore.h \
    initparameters.h \
    netphotomaster.h \
    $$PWD/modbus/modbusregister.h \
    $$PWD/modbus/tcpmastersingle.h \
    $$PWD/modbus/dubbus.h \
    $$PWD/xmlpro/project.h \
    $$PWD/xmlpro/subsystem.h \
    $$PWD/xmlpro/signal.h \
    $$PWD/xmlpro/driver.h \
    $$PWD/xmlpro/mmodbus.h \
    $$PWD/xmlpro/variable.h \
    $$PWD/xmlpro/device.h \
    $$PWD/xmlpro/def.h \
    $$PWD/xmlpro/save.h \
    $$PWD/xmlpro/logger.h \
    $$PWD/xmlpro/register.h \
    $$PWD/xmlpro/tablemodbus.h

FORMS += \
    mainwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += $$PWD/xmlpro
DEPENDPATH += $$PWD/xmlpro

INCLUDEPATH += $$PWD/modbus
DEPENDPATH += $$PWD/modbus

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libmodbus
