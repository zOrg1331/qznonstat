#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datakeeper.h"
#include "distanceelement.h"
#include "cluster.h"
#include "ardataanalysis.h"
#include "armodeltune.h"
#include "splitsclusterisation.h"
#include "splitsclusterisationtune.h"

#include <QFileDialog>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeSeriesesTabs->removeTab(0);

    ui->dataAnalysisTabs->setTabText(0, trUtf8("Расчет \"координат\" объектов кластеризации"));
    ui->dataAnalysisTabs->setTabText(1, trUtf8("Кластеризация"));
    ui->dataAnalysisTabs->setTabText(2, trUtf8("Сохранение отчета"));

    progress = new QProgressDialog(trUtf8("Идет расчет..."), "Отмена", 0, 1, this);
    progress->setWindowModality(Qt::ApplicationModal);
    progress->setCancelButton(0);
    progress->setMinimumDuration(1000);

    arDataAnalysisRoutine = new ArDataAnalysis();
    arDataAnalysisRoutine->setDataKeepers(&dataKeepers);
    arDataAnalysisRoutine->setDistanceElements(&distanceElements);

    arModelTune = new ArModelTune(arDataAnalysisRoutine, this);
    arModelTune->hide();

    splitsClusterisationRoutine = new SplitsClusterisation();
    splitsClusterisationRoutine->setDataKeepers(&dataKeepers);
    splitsClusterisationRoutine->setDistanceElements(&distanceElements);
    splitsClusterisationRoutine->setClusters(&clusters);

    splitsClusterisationTune = new SplitsClusterisationTune(splitsClusterisationRoutine, this);
    splitsClusterisationTune->hide();
}

MainWindow::~MainWindow()
{
    disconnect(this);
    while (!dataKeepers.isEmpty())
        delete dataKeepers.takeFirst();
    delete splitsClusterisationRoutine;
    delete arDataAnalysisRoutine;
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
        if (dataKeeper->getDataNormalize()) ui->dataNormalizeCheck->setChecked(true);
        else ui->dataNormalizeCheck->setChecked(false);
        ui->fileNameLabel->setText(trUtf8("Файл: %1")
                                   .arg(dataKeeper->getDataFileNameShort()));
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
    ui->dataNormalizeCheck->setDisabled(disable);
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
                                                    trUtf8("Временные ряды (*.txt *.dat)"));
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


void MainWindow::on_dataNormalizeCheck_clicked(bool checked)
{
    DataKeeper *dataKeeper = dataKeepers[ui->timeSeriesesTabs->currentIndex()];
    if (!dataKeeper) return;
    dataKeeper->setDataNormalize(checked);
}

void MainWindow::on_doDataAnalysisButton_clicked()
{
    switch (ui->dataAnalysisMethodBox->currentIndex()) {
    case 0:
        progress->setMaximum(arDataAnalysisRoutine->getEstimatedTime());
        disconnect(progress);
        connect(arDataAnalysisRoutine, SIGNAL(progressStep(int)),
                progress, SLOT(setValue(int)));
        ui->dataAnalysisSetupMethodButton->setEnabled(false);
        connect(arDataAnalysisRoutine, SIGNAL(finished()),
                this, SLOT(enable_dataAnalysisSetupMethodButton()));
        connect(arDataAnalysisRoutine, SIGNAL(finished()),
                this, SLOT(enable_clusterSetupMethodButton()));
        arDataAnalysisRoutine->setParams(ui->dataAnalysisParam1Edit->text().toInt(),
                                         ui->dataAnalysisParam2Edit->text().toInt());
        arDataAnalysisRoutine->start();
        break;
    }
}

void MainWindow::enable_dataAnalysisSetupMethodButton()
{
    ui->dataAnalysisSetupMethodButton->setEnabled(true);
}

void MainWindow::enable_clusterSetupMethodButton()
{
    ui->clusterSetupMethodButton->setEnabled(true);
}

void MainWindow::on_dataAnalysisSetupMethodButton_clicked()
{
    switch (ui->dataAnalysisMethodBox->currentIndex()) {
    case 0:
        arModelTune->show();
        break;
    }
}

void MainWindow::on_clusterSetupMethodButton_clicked()
{
    switch (ui->clusterMethodBox->currentIndex()) {
    case 0:
        splitsClusterisationTune->show();
        break;
    }
}

void MainWindow::on_saveReportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Сохранить отчет"),
                                                    "", trUtf8("Текстовые файлы (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);

    out << trUtf8("Отчет по сравнению временных рядов с помощью кластеризации (методом последовательных разбиений).\n");
    out << "\n";
    out << trUtf8("сравниваемые временные ряды:\n");
    for (int i = 0; i < splitsClusterisationRoutine->getDataKeepers()->size(); i++) {
        out << trUtf8("ряд%1: ").arg(i) << splitsClusterisationRoutine->getDataKeepers()->at(i)->getDataFileName() << "\n";
    }
    out << trUtf8("параметры:\n");
    out << trUtf8("данные:\n");
    for (int i = 0; i < splitsClusterisationRoutine->getDataKeepers()->size(); i++) {
        out << trUtf8("        ряд%1 от: ").arg(i) << splitsClusterisationRoutine->getDataKeepers()->at(i)->getDataFrom()
            << trUtf8(" до: ") << splitsClusterisationRoutine->getDataKeepers()->at(i)->getDataTo() << "\n";
        out << trUtf8("        окно: ") << splitsClusterisationRoutine->getDataKeepers()->at(i)->getDataWindow() << "\n";
        out << trUtf8("        смещение: ") << splitsClusterisationRoutine->getDataKeepers()->at(i)->getDataWindowStep() << "\n";
    }
    out << trUtf8("метод размерность: ") << arDataAnalysisRoutine->getDimension() << "\n";
    out << trUtf8("      порядок: ") << arDataAnalysisRoutine->getOrder() << "\n";
    out << trUtf8("кластеризация, разделение на кластеры: \n");
    out << splitsClusterisationRoutine->getClustersReport();
    
    file.close();
}
