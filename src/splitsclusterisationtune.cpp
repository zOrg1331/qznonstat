#include "splitsclusterisationtune.h"
#include "ui_splitsclusterisationtune.h"

#include "distanceelement.h"
#include "datakeeper.h"
#include "nsclusteritem.h"

SplitsClusterisationTune::SplitsClusterisationTune(SplitsClusterisation *splitsClusterisation, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplitsClusterisationTune),
    splitsClusterisation(splitsClusterisation)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window);

    ui->clustersGraphView->setScene(&partsScene);
    ui->clustersGraphView->setRenderHint(QPainter::Antialiasing, true);
    
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
    
    ui->partsCntEdit->setValue(splitsClusterisation->getSplits());
}

void SplitsClusterisationTune::on_drawClustersGraph_clicked()
{
    clustersVector.clear();
    
    int fromParts = ui->fromPartsCntEdit->text().toInt();
    int toParts = ui->toPartsCntEdit->text().toInt();
    for (int i = fromParts; i <= toParts; i++) {
        splitsClusterisation->setParams(i, ui->minPartsCntEdit_2->text().toInt());
        splitsClusterisation->start();
        splitsClusterisation->wait();
        
        clustersVector.append(*splitsClusterisation->getClusters());
    }
    
    updateClustersGraph();
}

void SplitsClusterisationTune::updateClustersGraph()
{
    partsScene.clear();

    int maxClusterWidth = 40;
    int clustersMargin = 10;
    int maxClusterElements = splitsClusterisation->getDistanceElements()->size();
    
    int fromParts = ui->fromPartsCntEdit->text().toInt();

    NSClusterItem *zeroItem = new NSClusterItem;
    zeroItem->setRect(0, 0/*-maxClusterWidth/2*/,
                      maxClusterWidth*(maxClusterElements/(double)maxClusterElements),
                      maxClusterWidth*(maxClusterElements/(double)maxClusterElements));
    zeroItem->setPen(QPen(Qt::black));
    zeroItem->setBrush(QBrush(Qt::darkRed));
    zeroItem->partsCnt = 0;
    zeroItem->clusterNum = 0;
    partsScene.addItem(zeroItem);

    for (int partI = 0; partI < clustersVector.size(); partI++) {
        
        partsScene.addText(QString("%1").arg(partI+fromParts))->setPos((partI+1)*(maxClusterWidth+clustersMargin),
                                                               maxClusterWidth+5);
 
        const QMap<int, QVector<DistanceElement> > *clusters = &clustersVector.at(partI);

        int i = 0;
        foreach (int cluster, clusters->keys()) {
            NSClusterItem *item = new NSClusterItem;
            item->setRect((partI+1)*(maxClusterWidth+clustersMargin),
                          -i*(maxClusterWidth+clustersMargin),
                          maxClusterWidth*(clusters->value(cluster).size()/(double)maxClusterElements),
                          maxClusterWidth*(clusters->value(cluster).size()/(double)maxClusterElements));
            item->setPen(QPen(Qt::black));
            item->setBrush(QBrush(Qt::darkRed));
            item->partsCnt = partI;
            item->clusterNum = cluster;
            partsScene.addItem(item);
            i++;
        }

        if (partI >= 1) {
            const QMap<int, QVector<DistanceElement> > *clustersPrev = &clustersVector.at(partI-1);

            int distanceElementsPrevCntMax = 0;
            foreach (int clusterPrev, clustersPrev->keys()) {
                if (clustersPrev->value(clusterPrev).size() > distanceElementsPrevCntMax)
                    distanceElementsPrevCntMax = clustersPrev->value(clusterPrev).size();
            }
            
            foreach (int clusterPrev, clustersPrev->keys()) {
                // номер кластера, в который попали distElementOld и количество попавших туда элементов
                QMap<int, int> linesCount;

                // пробегаемся по всем элементам текущего кластера
                int distanceElementsPrevCnt = clustersPrev->value(clusterPrev).size();
                for (int distElementPrev = 0; distElementPrev < distanceElementsPrevCnt; distElementPrev++) {
                    DistanceElement distElementTmp = clustersPrev->value(clusterPrev).at(distElementPrev);

                    // ищем этот элемент в кластерах clustersI
                    foreach (int cluster, clusters->keys()) {
                        int distanceElementsCnt = clusters->value(cluster).size();
                        bool isFinished = false;
                        for (int distElementI = 0; distElementI < distanceElementsCnt; distElementI++) {
                            if (distElementTmp == clusters->value(cluster).at(distElementI)) {
                                // элемент distElementTmp из кластера clustersIOldIter.key() найден
                                // в кластере clustersIIter.key()
                                // соединяем эти кластера линией и прерываем процесс
                                if (!linesCount.contains(cluster)) {
                                    linesCount[cluster] = 0;
                                } else {
                                    linesCount[cluster]++;
                                }
                                isFinished = true;
                                break;
                            }
                        }
                        if (isFinished) break;
                    }
                }

                foreach (int line, linesCount.keys()) {
                    QPointF oldClusterPos;
                    int oldClusterWidth;
                    QPointF clusterPos;
                    int clusterWidth;
                    
                    QList<QGraphicsItem *> items = partsScene.items();
                    for (int i = 0; i < items.size(); i++) {
                        NSClusterItem *item = qgraphicsitem_cast<NSClusterItem*>(items.at(i));
                        if (item) {
                            if ((item->partsCnt == (partI-1)) && (item->clusterNum == clusterPrev)) {
                                oldClusterPos = item->rect().topLeft();
                                oldClusterWidth = item->rect().width();
                            }
                            if ((item->partsCnt == partI) && (item->clusterNum == line)) {
                                clusterPos = item->rect().topLeft();
                                clusterWidth = item->rect().width();
                            }
                        }
                    }
                    partsScene.addLine(oldClusterPos.x()+oldClusterWidth/2,
                                       oldClusterPos.y()+oldClusterWidth/2,
                                       clusterPos.x()+clusterWidth/2,
                                       clusterPos.y()+clusterWidth/2,
                                       QPen(Qt::black,
                                            5.0*(double)linesCount.value(line)/(double)distanceElementsPrevCntMax));
                }
            }
        }
    }
}

void SplitsClusterisationTune::on_clustersTable_cellClicked(int row, int column)
{
    if (column <= 1) return;
    
    int tsNum = column - 2;
    int clusterNum = ui->clustersTable->item(row, 0)->text().toInt();
    
    QVector<DistanceElement> distanceElements = splitsClusterisation->getClusters()->value(clusterNum);
    QVector<int> parts;
    for (int i = 0; i < distanceElements.size(); i++) {
        if (distanceElements.at(i).getTsNum() == tsNum) parts.append(distanceElements.at(i).getWindowNum());
    }
    
    splitsClusterisation->getDataKeepers()->at(tsNum)->showParts(parts);
}
