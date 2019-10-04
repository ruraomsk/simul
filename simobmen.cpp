#include "simobmen.h"


SimObmen::SimObmen(DataStore *dts, QWidget *parent)
{
    QStringList header;
    QDateTime now=QDateTime::currentDateTime();
    tree=new QTreeWidget();
    tree->tr("Simul");
    this->dts=dts;
    this->parent=parent;
    header<< "Подсистема/устройство"<<"Описание"<<"Текущее значение"<<"Key";

    QTreeWidgetItem *itm=new QTreeWidgetItem();
    itm->setText(0,"Обмен данными");
    tree->setColumnCount(3);
    tree->addTopLevelItem(itm);
    foreach (QString nameDev, dts->alldevs.keys()) {
        Device dev=dts->alldevs.value(nameDev);
        QTreeWidgetItem *child=new QTreeWidgetItem();
        child->setText(0,nameDev);
        child->setText(1,dev.description);
        child->setText(3,nameDev);
        if(dev.lastOperation.secsTo(now)>Setup.TimeReady) child->setTextColor(0,Qt::red);
        else child->setTextColor(0,Qt::darkGreen);
        QFont font("Helvetica",12,QFont::Bold);
        child->setFont(0,font);
        child->setFont(1,font);
        foreach (QString nameVar, dev.myvars.keys()) {
            QTreeWidgetItem *child1=new QTreeWidgetItem();
            Variable var=dev.allvars->value(nameVar);
            child1->setText(0,nameVar);
            child1->setText(1,var.description);
            child1->setText(2,var.value);
            child1->setText(3,nameDev+":"+nameVar);
            child->addChild(child1);
        }
        itm->addChild(child);
    }
    tree->setHeaderLabels(header);
        tmr=new QTimer(this);
        tmr->setInterval(1000);
        tmr->start();
        fastTimer=new QTimer(this);
        fastTimer->setInterval(100);
        fastTimer->start();
        connect(tmr, &QTimer::timeout,this,&SimObmen::updateTimer);
}

void SimObmen::updateTimer()
{
    QDateTime now=QDateTime::currentDateTime();
    QTreeWidgetItemIterator it(tree,QTreeWidgetItemIterator::All);
    while(*(++it)){
        QString key=(*it)->text(3);
        QStringList keys=key.split(":");
        if(keys.count()<2||keys.count()>3){
            continue;
        }
        QString nameDev=keys[0]+":"+keys[1];
        Device dev=dts->alldevs.value(nameDev);
        if(keys.count()==2){
            //Это устройство вводы вывода
            if(dev.lastOperation.secsTo(now)>Setup.TimeReady) (*it)->setTextColor(0,Qt::red);
            else (*it)->setTextColor(0,Qt::darkGreen);
        }
        if(keys.count()==3){
            //Это переменная на устройстве вводы вывода
            Variable var=dev.allvars->value(keys[2]);
            (*it)->setText(2,var.value);
        }

    }

}

void SimObmen::showFinded(QStringList seek)
{
    QTreeWidgetItemIterator it(tree,QTreeWidgetItemIterator::All);
    if(seek.count()==0){
        while(*(++it)){
            (*it)->setHidden(false);
        }
        tree->expandAll();
        return;
    }
    while(*(++it)){
        QString key=(*it)->text(3);
        QStringList keys=key.split(":");
        if(keys.count()!=3){
            continue;
        }
        if(isFinded(seek,(*it)->text(0),(*it)->text(1))) (*it)->setHidden(false);
        else (*it)->setHidden(true);
    }
    tree->expandAll();
}

void SimObmen::setSize(int w, int h)
{
    tree->resize(w,h);
    tree->hideColumn(3);
    tree->expandAll();
    for (int i = 0; i < tree->columnCount(); ++i) {
        tree->resizeColumnToContents(i);
    }
    tree->collapseAll();
}


void SimObmen::connectSlotFind(VariableFinder *varFind)
{
    connect(varFind,&VariableFinder::on_find_clicked,this,&SimObmen::showFinded);
}

void SimObmen::connectCollapsFind(VariableFinder *varFind)
{
    connect(varFind,&VariableFinder::collapsAll,this,&SimObmen::collaps);

}

bool SimObmen::isFinded(QStringList seek, QString one, QString duo)
{
    foreach (QString str, seek) {
        if(one.toUpper().contains(str.toUpper())) return true;
        if(duo.toUpper().contains(str.toUpper())) return true;
    }
    return false;
}

void SimObmen::collaps()
{
    tree->collapseAll();
}
