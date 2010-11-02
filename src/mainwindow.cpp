#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datakeeper.h"
#include "ardataanalysis.h"

#include <QFileDialog>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeSeriesesTabs->removeTab(0);

    progress = new QProgressDialog(trUtf8("Идет расчет..."), "Отмена", 0, 1, this);
    progress->setWindowModality(Qt::ApplicationModal);
    progress->setCancelButton(0);
    progress->setMinimumDuration(1000);

    arDataAnalysisRoutine = new ArDataAnalysis();
    arDataAnalysisRoutine->setDataKeepers(&dataKeepers);
    connect(arDataAnalysisRoutine, SIGNAL(progressStep(int)),
            progress, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillDataSettingsWidgets(DataKeeper *dataKeeper)
{
    if (dataKeeper) {
        ui->dataFromEdit->setText(QString::number(dataKeeper->getDataFrom()));
        ui->dataToEdit->setText(QString::number(dataKeeper->getDataTo()));
        ui->dataWindowEdit->setText(QString::number(dataKeeper->getDataWindow()));
        ui->dataStepEdit->setText(QString::number(dataKeeper->getDataWindowStep()));
        ui->dataSamplingEdit->setText(QString::number(dataKeeper->getDataSampling()));
        if (dataKeeper->getDataUseFullTs()) ui->dataAllCheck->setChecked(true);
        else ui->dataAllCheck->setChecked(false);
        if (dataKeeper->getDataUseSeconds()) ui->dataSecondsCheck->setChecked(true);
        else ui->dataSecondsCheck->setChecked(false);
        ui->fileNameLabel->setText(trUtf8("Файл: %1")
                                   .arg(QFileInfo(dataKeeper->getDataFileName()).baseName()));
    }
}

void MainWindow::disableDataSettingsWidgets(bool disable) {
    ui->loadDataButton->setDisabled(disable);
    ui->dataFromEdit->setDisabled(disable);
    ui->dataToEdit->setDisabled(disable);
    ui->dataWindowEdit->setDisabled(disable);
    ui->dataStepEdit->setDisabled(disable);
    ui->dataAllCheck->setDisabled(disable);
    ui->dataSecondsCheck->setDisabled(disable);
    ui->dataSamplingEdit->setDisabled(disable);
}

void MainWindow::on_timeSeriesesTabs_currentChanged(int index)
{
    if (dataKeepers.size() > 0) {
        disableDataSettingsWidgets(false);

        DataKeeper *dataKeeper = dataKeepers[index];
        if (!dataKeeper) return;
        fillDataSettingsWidgets(dataKeeper);
    } else {
        disableDataSettingsWidgets(true);
    }
}

void MainWindow::on_addDataButton_clicked()
{
    DataKeeper *dataKeeper = new DataKeeper();
    dataKeeper->setNum(ui->timeSeriesesTabs->count());

    dataKeepers.append(dataKeeper);

    ui->timeSeriesesTabs->addTab(dataKeeper->getWidget(),
                                 dataKeeper->getDescription());
    ui->timeSeriesesTabs->setCurrentIndex(ui->timeSeriesesTabs->count()-1);

    fillDataSettingsWidgets(dataKeeper);
}

void MainWindow::on_timeSeriesesTabs_tabCloseRequested(int index)
{
    delete dataKeepers[index];
    dataKeepers.removeAt(index);
    refreshDataTabsText();
}

void MainWindow::on_loadDataButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    trUtf8("Открыть временной ряд"),
                                                    "",
                                                    tr("Временные ряды (*.txt *.dat)"));
    if (!fileName.isEmpty()) {
        ui->fileNameLabel->setText(trUtf8("Файл: %1")
                                   .arg(QFileInfo(fileName).baseName()));
        dataKeepers[ui->timeSeriesesTabs->currentIndex()]->setDataFileName(fileName);
        refreshDataTabsText();
    }
}

void MainWindow::refreshDataTabsText()
{
    for (int i = 0; i < dataKeepers.size(); i++) {
        dataKeepers[i]->setNum(i);
        ui->timeSeriesesTabs->setTabText(i,
                                         dataKeepers.at(i)->getDescription());
    }
}

void MainWindow::on_dataFromEdit_editingFinished()
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataFrom(ui->dataFromEdit->text().toInt());
    dataKeeper->redrawData();
}

void MainWindow::on_dataToEdit_editingFinished()
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataTo(ui->dataToEdit->text().toInt());
    dataKeeper->redrawData();
}

void MainWindow::on_dataWindowEdit_editingFinished()
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataWindow(ui->dataWindowEdit->text().toInt());
    dataKeeper->redrawData();
}

void MainWindow::on_dataStepEdit_editingFinished()
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataWindowStep(ui->dataStepEdit->text().toInt());
    dataKeeper->redrawData();
}

void MainWindow::on_dataSamplingEdit_editingFinished()
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataSampling(ui->dataSamplingEdit->text().toInt());
    dataKeeper->redrawData();
}

void MainWindow::on_dataAllCheck_clicked(bool checked)
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataUseFullTs(checked);
    dataKeeper->redrawData();
}

void MainWindow::on_dataSecondsCheck_clicked(bool checked)
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataUseSeconds(checked);
    dataKeeper->redrawData();
}

void MainWindow::on_doDataAnalysisButton_clicked()
{
    switch (ui->dataAnalysisMethodBox->currentIndex()) {
    case 0:
        progress->setMaximum(arDataAnalysisRoutine->getEstimatedTime());
        progress->setValue(0);
        arDataAnalysisRoutine->setParams(ui->dataAnalysisParam1Edit->text().toInt(),
                                         ui->dataAnalysisParam2Edit->text().toInt());
        arDataAnalysisRoutine->start();
        break;
    }
}
