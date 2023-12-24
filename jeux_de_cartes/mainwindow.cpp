#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "second.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    second(this)
{
    ui->setupUi(this);
    QPushButton *myButton = findChild<QPushButton*>("bt1");

    if (myButton) {
        myButton->setStyleSheet("QPushButton {"
                                "background-color: #4CAF50;"
                                "border-radius: 10px; "
                                "border: 1.5px solid rgb(91,231,255); "
                                "color: white;"
                                "text-align: center;"
                                "text-decoration: none;"
                                "font-size: 30px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #FFA07A;"
                                "color: black;"
                                "font-size: 20px;"
                                "}");
    }

    QPushButton *myButton2 = findChild<QPushButton*>("bt2");

    if (myButton2) {
        myButton2->setStyleSheet("QPushButton {"
                                "background-color: #4CAF50;"
                                "border-radius: 10px; "
                                "border: 1.5px solid rgb(91,231,255); "
                                "color: white;"
                                "text-align: center;"
                                "text-decoration: none;"
                                "font-size: 30px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #FFA07A;"
                                "color: black;"
                                "font-size: 20px;"
                                "}");
    }
    ui->widget->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_bt2_clicked()
{
    second.show();
    second.showFullScreen();
    this->hide();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QPixmap bkgnd(":/background.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    QWidget::resizeEvent(event);
}

void MainWindow::on_bt1_clicked()
{
    ui->widget->show();
}

void MainWindow::on_buttonBox_accepted()
{
    this->close();
    second.close();
}

void MainWindow::on_buttonBox_rejected()
{
    ui->widget->close();
}

