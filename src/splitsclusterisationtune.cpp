#include "splitsclusterisationtune.h"
#include "ui_splitsclusterisationtune.h"

#include "distanceelement.h"
#include "datakeeper.h"

SplitsClusterisationTune::SplitsClusterisationTune(SplitsClusterisation *splitsClusterisation, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplitsClusterisationTune),
    splitsClusterisation(splitsClusterisation)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window);

    progress = new QProgressDialog(trUtf8("Идет расчет..."), trUtf8("Отмена"), 0, 1, this);
    progress->setWindowModality(Qt::ApplicationModal);
    progress->setMinimumDuration(0);
    
    connect(splitsClusterisation, SIGNAL(finished()),
            this, SLOT(updateClustersTable()));
}

SplitsClusterisationTune::~SplitsClusterisationTune()
{
    delete ui;
}

void SplitsClusterisationTune::on_calcClustersButton_clicked()
{
    splitsClusterisation->setParams(ui->partsCntEdit->text().toInt(),
                                    ui->minPartsCntEdit_2->text().toInt());
    splitsClusterisation->start();
}

void SplitsClusterisationTune::updateClustersTable()
{
    int row = 0;
    QMap<int, QVector<DistanceElement> > *clusters;
    int tsCount = 0;

    clusters = splitsClusterisation->getClusters();
    tsCount = splitsClusterisation->getDataKeepers()->size();
    
    ui->clustersTable->setRowCount(clusters->size());
    ui->clustersTable->setColumnCount(tsCount + 2);
    
    QVector<int> partsCount;
    partsCount.resize(tsCount);

    foreach (int cluster, clusters->keys()) {
        QTableWidgetItem *newItem0 = new QTableWidgetItem(QString("%1").arg(cluster));
        ui->clustersTable->setItem(row, 0, newItem0);
        
        QVector<DistanceElement> distanceElements = clusters->value(cluster);

        QTableWidgetItem *newItem1 = new QTableWidgetItem(QString("%1").arg(distanceElements.size()));
        ui->clustersTable->setItem(row, 1, newItem1);
        
        partsCount.fill(0);
        for (int i = 0; i < distanceElements.size(); i++) {
            partsCount[distanceElements.at(i).getTsNum()]++;
        }
        
        for (int i = 0; i < tsCount; i++) {
            QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(partsCount.at(i)));
            ui->clustersTable->setItem(row, 2+i, newItem);
            QString name = QFileInfo(splitsClusterisation->getDataKeepers()->at(i)->getDataFileName()).baseName();
            ui->clustersTable->setHorizontalHeaderItem(2+i, new QTableWidgetItem(name));
        }
        
        row++;
    }
    ui->clustersTable->resizeColumnsToContents();
}
