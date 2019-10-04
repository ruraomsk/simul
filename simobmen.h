#ifndef SIMOBMEN_H
#define SIMOBMEN_H

#include <QWidget>
#include <QTreeWidget>
#include <QTimer>
#include <QDebug>

#include "datastore.h"
#include "variablefinder.h"
#include "initparameters.h"

extern IniParam Setup;

class SimObmen : public QWidget
{
    Q_OBJECT
public:
    explicit SimObmen(DataStore *dts,QWidget *parent = nullptr);
    void updateTimer();
    void showFinded(QStringList seek);
    void setSize(int w, int h);
    void connectSlotFind(VariableFinder *varFind);
    void connectCollapsFind(VariableFinder *varFind);
    QTreeWidget *tree;
private:
    QTimer *tmr=nullptr;
    QTimer *fastTimer=nullptr;

    DataStore *dts;
    QWidget *parent;
    bool isFinded(QStringList seek,QString one,QString duo);
    void collaps();
signals:

public slots:
};

#endif // SIMOBMEN_H
