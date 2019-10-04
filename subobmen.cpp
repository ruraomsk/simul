#include "subobmen.h"

SubObmen::SubObmen(DataStore *dts,QWidget *parent) : QWidget(parent)
{
    QStringList header;
            NetPhotoMaster *master0;
            NetPhotoMaster *master1;
    QDateTime now=QDateTime::currentDateTime();
    tree=new QTreeWidget();
    tree->tr("Subsystems");
    this->dts=dts;
    this->parent=parent;
    header<< "Подсистема/Переменная"<<"Описание"<<"Текущее значение"<<"Key";

    QTreeWidgetItem *itm=new QTreeWidgetItem();
    itm->setText(0,"Подсистемы");
    tree->setColumnCount(3);
    tree->addTopLevelItem(itm);
    foreach (QString nameSub, dts->pr.subs.keys()) {
        Subsystem sub=dts->pr.subs.value(nameSub);
        QTreeWidgetItem *child=new QTreeWidgetItem();
        child->setText(0,nameSub);
        child->setText(1,sub.description);
        child->setText(3,nameSub);
        QFont font("Helvetica",12,QFont::Bold);
        child->setFont(0,font);
        child->setFont(1,font);

        master0=new NetPhotoMaster(dts,nameSub,0);
        master1=new NetPhotoMaster(dts,nameSub,1);
        masters.insert(nameSub+":0",master0);
        masters.insert(nameSub+":1",master1);

        foreach (QString nameVar, sub.vars.keys()) {
            QTreeWidgetItem *child1=new QTreeWidgetItem();
            Variable var=sub.vars.value(nameVar);
            child1->setText(0,nameVar);
            child1->setText(1,var.description);
            child1->setText(2,var.value);
            child1->setText(3,nameSub+":"+nameVar);
            child->addChild(child1);
        }
        itm->addChild(child);
    }
    tree->setHeaderLabels(header);
    tree->hideColumn(3);
    tmr=new QTimer();
    tmr->setInterval(Setup.Screen_Update);
    tmr->start();
    connect(tmr, &QTimer::timeout,this,&SubObmen::updateTimer);
    foreach (NetPhotoMaster *master, masters) {
        master->start();
        while (!master->isRunning()){
            QThread::sleep(100);
        }
    }
}
void SubObmen::updateTimer()
{
    QDateTime now=QDateTime::currentDateTime();
    QTreeWidgetItemIterator it(tree,QTreeWidgetItemIterator::All);
    NetPhotoMaster *master=nullptr;
    while(*(++it)){
        QString key=(*it)->text(3);
        QStringList keys=key.split(":");
        if(keys.count()<1||keys.count()>2){
            continue;
        }
        QString nameSub=keys[0];
        Subsystem sub=dts->pr.subs.value(nameSub);
        if(keys.count()==1){
            //Это подсистема
            NetPhotoMaster *m0=masters.value(nameSub+":0");
            NetPhotoMaster *m1=masters.value(nameSub+":1");
            master=(m0->getLastTime()>m1->getLastTime())?m0:m1;
            if(master->getLastTime().secsTo(now)>Setup.TimeReady) (*it)->setTextColor(0,Qt::red);
            else (*it)->setTextColor(0,Qt::darkGreen);
        }
        if(keys.count()==2){
            //Это переменная подсистемы
            (*it)->setText(2,master->readVariable(keys[1]));
        }

    }

}

void SubObmen::showFinded(QStringList seek)
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
        if(keys.count()!=2) continue;
        if(isFinded(seek,(*it)->text(0),(*it)->text(1))) (*it)->setHidden(false);
        else (*it)->setHidden(true);
    }
    tree->expandAll();

}
void SubObmen::setSize(int w, int h)
{
    tree->resize(w,h);
    tree->expandAll();
    for (int i = 0; i < tree->columnCount(); ++i) {
        tree->resizeColumnToContents(i);
    }
    tree->collapseAll();
}


void SubObmen::connectSlotFind(VariableFinder *varFind)
{
    connect(varFind,&VariableFinder::on_find_clicked,this,&SubObmen::showFinded);

}

void SubObmen::connectCollapsFind(VariableFinder *varFind)
{
    connect(varFind,&VariableFinder::collapsAll,this,&SubObmen::collaps);

}

bool SubObmen::isFinded(QStringList seek, QString one, QString duo)
{
    foreach (QString str, seek) {
        if(one.toUpper().contains(str.toUpper())) return true;
        if(duo.toUpper().contains(str.toUpper())) return true;
    }
    return false;
}

void SubObmen::collaps()
{
    tree->collapseAll();
}
