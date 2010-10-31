#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datakeeper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->timeSeriesesTabs->removeTab(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addDataButton_clicked()
{
    DataKeeper *dataKeeper = new DataKeeper();
    dataKeeper->setNum(ui->timeSeriesesTabs->count());

    ui->timeSeriesesTabs->addTab(dataKeeper->getWidget(),
                                 dataKeeper->getDescription());
    ui->timeSeriesesTabs->setCurrentIndex(ui->timeSeriesesTabs->count()-1);
}
