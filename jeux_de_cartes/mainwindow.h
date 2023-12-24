#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include "second.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_bt2_clicked();
    void on_bt1_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::MainWindow *ui;
    second second;


};
#endif // MAINWINDOW_H
