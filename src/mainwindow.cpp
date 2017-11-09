#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage img("C:/Users/Public/Pictures/Sample Pictures/Lighthouse.jpg");
    ui->widget->setImage(img);
}

MainWindow::~MainWindow()
{
    delete ui;
}
