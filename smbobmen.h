#ifndef SMBOBMEN_H
#define SMBOBMEN_H

#include <QWidget>
#include <QTreeWidget>
#include <QDebug>
#include <QTimer>

#include "datastore.h"
#include "variablefinder.h"
#include "initparameters.h"
#include "dubbus.h"

extern IniParam Setup;

class SmbObmen : public QWidget
{
    Q_OBJECT
public:
    explicit SmbObmen(DataStore *dts,QWidget *parent = nullptr);
    void updateTimer();
    void showFinded(QStringList seek);

    void setSize(int w, int h);
    void connectSlotFind(VariableFinder *varFind);
    void connectCollapsFind(VariableFinder *varFind);

    QTreeWidget *tree;

private:
    QTimer *tmr;
    DataStore *dts;
    QMap<QString,DubBus> dubs;
    QWidget *parent;
    bool isFinded(QStringList seek,QString one,QString duo);
    void collaps();

};

#endif // SMBOBMEN_H
