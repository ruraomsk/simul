#ifndef VARIABLEFINDER_H
#define VARIABLEFINDER_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>

class VariableFinder:public QWidget
{
    Q_OBJECT
public:
    explicit VariableFinder(QWidget *parent=nullptr);
    QStringList seek();

    QFrame *frame;

signals:
    void on_find_clicked(QStringList text);
    void collapsAll();

private:
    QPushButton *findButton;
    QPushButton *collapsButton;
    QLineEdit *lineEdit;
    QString seek_str="";
    QWidget  *parent;
    void clicked();
    void collaps();
};

#endif // VARIABLEFINDER_H
