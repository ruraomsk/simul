#include "smbobmen.h"


SmbObmen::SmbObmen(DataStore *dts, QWidget *parent)
{
    QStringList header;
    QDateTime now=QDateTime::currentDateTime();
    tree=new QTreeWidget();
    tree->tr("Modbuses");
    this->dts=dts;
    this->parent=parent;
    header<< "Подсистема/ModBus/Переменная"<<"Описание"<<"Текущее значение"<<"Key";

    QTreeWidgetItem *itm=new QTreeWidgetItem();
    itm->setText(0,"Подсистемы");
    tree->setColumnCount(3);
    tree->addTopLevelItem(itm);
    foreach (QString nameSub, dts->pr.subs.keys()) {
        Subsystem sub=dts->pr.subs.value(nameSub);
        foreach (MModbus mb, sub.modbuses) {
            QTreeWidgetItem *child=new QTreeWidgetItem();
            if(mb.type!="slave") continue;
            DubBus db=DubBus(&sub,mb);
            dubs.insert(nameSub+":"+mb.name,db);
            child->setText(0,nameSub+"/"+mb.name);

            child->setText(1,mb.description);
            QFont font("Helvetica",12,QFont::Bold);
            child->setFont(0,font);
            child->setFont(1,font);

            child->setText(3,nameSub+":"+mb.name);

            if(!db.isConnected()) child->setTextColor(0,Qt::red);
            else child->setTextColor(0,Qt::darkGreen);
            foreach (Register reg,mb.tableModbus.registers) {
                QTreeWidgetItem *child1=new QTreeWidgetItem();
                child1->setText(0,reg.name);
                child1->setText(1,reg.description);
                child1->setText(2,db.getValue(reg.name));
                child1->setText(3,nameSub+":"+mb.name+":"+reg.name);
                child->addChild(child1);
            }
            itm->addChild(child);
        }

    }
    tree->setHeaderLabels(header);
    tree->hideColumn(3);
    tmr=new QTimer();
    tmr->setInterval(Setup.Screen_Update);
    tmr->start();
    connect(tmr, &QTimer::timeout,this,&SmbObmen::updateTimer);

}

void SmbObmen::updateTimer()
{
    QTreeWidgetItemIterator it(tree,QTreeWidgetItemIterator::All);
    DubBus dub;
    while(*(++it)){
        QString key=(*it)->text(3);
        QStringList keys=key.split(":");
        if(keys.count()<2){
            continue;
        }
        if(keys.count()==2){
            //Это Dub Modbus
            dub=dubs.value(key);
            if(!dub.isConnected()) (*it)->setTextColor(0,Qt::red);
            else (*it)->setTextColor(0,Qt::darkGreen);
        }
        if(keys.count()==3){
            //Это переменная ModBus
            dub=dubs.value(keys[0]+":"+keys[1]);

            (*it)->setText(2,dub.getValue(keys[2]));
        }

    }

}

void SmbObmen::showFinded(QStringList seek)
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
        if(keys.count()!=3) continue;
        if(isFinded(seek,(*it)->text(0),(*it)->text(1))) (*it)->setHidden(false);
        else (*it)->setHidden(true);
    }
    tree->expandAll();



}

void SmbObmen::setSize(int w, int h)
{
    tree->resize(w,h);
    tree->expandAll();
    for (int i = 0; i < tree->columnCount(); ++i) {
        tree->resizeColumnToContents(i);
    }
    tree->collapseAll();

}

void SmbObmen::connectSlotFind(VariableFinder *varFind)
{
    connect(varFind,&VariableFinder::on_find_clicked,this,&SmbObmen::showFinded);
}

void SmbObmen::connectCollapsFind(VariableFinder *varFind)
{
    connect(varFind,&VariableFinder::collapsAll,this,&SmbObmen::collaps);
}

bool SmbObmen::isFinded(QStringList seek, QString one, QString duo)
{
    foreach (QString str, seek) {
        if(one.toUpper().contains(str.toUpper())) return true;
        if(duo.toUpper().contains(str.toUpper())) return true;
    }
    return false;
}

void SmbObmen::collaps()
{
    tree->collapseAll();
}
