#ifndef SUBOBMEN_H
#define SUBOBMEN_H

#include <QWidget>
#include <QTreeWidget>
#include <QTimer>
#include <QDebug>

#include "datastore.h"
#include "variablefinder.h"
#include "initparameters.h"
#include "netphotomaster.h"

extern IniParam Setup;

class SubObmen : public QWidget
{
    Q_OBJECT
public:
    explicit SubObmen(DataStore *dts,QWidget *parent = nullptr);
    void updateTimer();
    void showFinded(QStringList seek);

    void setSize(int w, int h);
    void connectSlotFind(VariableFinder *varFind);
    void connectCollapsFind(VariableFinder *varFind);

    QTreeWidget *tree;

private:
    QMap<QString,NetPhotoMaster*> masters;
    QTimer *tmr=nullptr;
    DataStore *dts;
    QWidget *parent;
    bool isFinded(QStringList seek,QString one,QString duo);
    void collaps();
signals:

public slots:
};

#endif // SUBOBMEN_H
