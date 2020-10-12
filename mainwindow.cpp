#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    detection = new Detection;
    detection->setStyleSheet("QWidget{padding: 0px;}");
    //this->setStyleSheet("QVBoxLayout::pane{border:1px solid black;padding: 0px;}");
    layoutMain = new QVBoxLayout;
    layoutMain->setMargin(0);
    //this->layout()->setMargin(0);
    layoutMain->addWidget(detection);
    ui->widgetMain->setLayout(layoutMain);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionDetection_triggered()
{
    detection->show();
}
