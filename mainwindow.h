#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    QTimer *tmr=nullptr;
    QTimer *fastTimer=nullptr;
private:
    Ui::MainWindow *ui;
private slots:
};

#endif // MAINWINDOW_H
