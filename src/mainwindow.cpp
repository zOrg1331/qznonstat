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

    dataKeepers.append(dataKeeper);

    ui->timeSeriesesTabs->addTab(dataKeeper->getWidget(),
                                 dataKeeper->getDescription());
    ui->timeSeriesesTabs->setCurrentIndex(ui->timeSeriesesTabs->count()-1);
}

void MainWindow::on_timeSeriesesTabs_tabCloseRequested(int index)
{
    delete dataKeepers[index];
    dataKeepers.removeAt(index);
    for (int i = 0; i < dataKeepers.size(); i++) {
        dataKeepers[i]->setNum(i);
        ui->timeSeriesesTabs->setTabText(i,
                                         dataKeepers.at(i)->getDescription());
    }
}
