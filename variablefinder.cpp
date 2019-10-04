#include "variablefinder.h"

VariableFinder::VariableFinder(QWidget *parent)
{
    this->parent=parent;
    frame=new QFrame();
    frame->resize(500,30);
    QHBoxLayout *layout=new QHBoxLayout();

    frame->setLayout(layout);

    lineEdit=new QLineEdit(seek_str);
    findButton=new QPushButton("Поиск");
    collapsButton=new QPushButton("Свернуть");

    lineEdit->resize(300,20);
    findButton->resize(100,20);
    collapsButton->resize(100,20);

    layout->addWidget(lineEdit);
    layout->addWidget(findButton);
    layout->addWidget(collapsButton);

    connect(lineEdit,&QLineEdit::returnPressed,this,&VariableFinder::clicked);
    connect(findButton,&QPushButton::clicked,this,&VariableFinder::clicked);
    connect(collapsButton,&QPushButton::clicked,this,&VariableFinder::collaps);
}

QStringList VariableFinder::seek()
{
    QStringList result;
    if(lineEdit->text().size()!=0) {
        result=lineEdit->text().split(";");
        if(result.value(result.size()-1)==""){
            result.removeLast();
        }
    }
    return result;
}

void VariableFinder::clicked()
{
    emit on_find_clicked(seek());
}

void VariableFinder::collaps()
{
    emit collapsAll();
}

