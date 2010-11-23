#include "splitsclusterisationtune.h"
#include "ui_splitsclusterisationtune.h"

#include "distanceelement.h"
#include "datakeeper.h"
#include "nsclusteritem.h"
#include "cluster.h"

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
    connect(splitsClusterisation, SIGNAL(finished()),
            this, SLOT(updateClustersDistributionTable()));
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
    QMap<int, Cluster* > *clusters;
    int tsCount = 0;

    clusters = splitsClusterisation->getClusters();
    tsCount = splitsClusterisation->getDataKeepers()->size();

    ui->clustersTable->setRowCount(clusters->size());
    ui->clustersTable->setColumnCount(tsCount + 2);

    QVector<int> partsDistribution;

    foreach (int cluster, clusters->keys()) {
        QTableWidgetItem *newItem0 = new QTableWidgetItem(QString("%1").arg(cluster));
        ui->clustersTable->setItem(row, 0, newItem0);

        int elementsCount = clusters->value(cluster)->getElementsCount();

        QTableWidgetItem *newItem1 = new QTableWidgetItem(QString("%1").arg(elementsCount));
        ui->clustersTable->setItem(row, 1, newItem1);

        partsDistribution = clusters->value(cluster)->getPartsDistribution();

        for (int i = 0; i < tsCount; i++) {
            int perc = 100*partsDistribution.at(i)
                    /splitsClusterisation->getDataKeepers()->at(i)->getWindowsCount();

            QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1 (%2\%)")
                                                             .arg(partsDistribution.at(i))
                                                             .arg(perc));
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

        const QMap<int, Cluster*> *clusters = &clustersVector.at(partI);

        int i = 0;
        foreach (int cluster, clusters->keys()) {
            int elementsCount = clusters->value(cluster)->getElementsCount();
            if (((ui->fivePercentLinesLimitBox->isChecked()) &&
                 ((double)elementsCount/(double)maxClusterElements > 0.05)) ||
                    (!ui->fivePercentLinesLimitBox->isChecked()))
            {
                NSClusterItem *item = new NSClusterItem;
                item->setRect((partI+1)*(maxClusterWidth+clustersMargin),
                              -i*(maxClusterWidth+clustersMargin),
                              maxClusterWidth*(elementsCount/(double)maxClusterElements),
                              maxClusterWidth*(elementsCount/(double)maxClusterElements));
                item->setPen(QPen(Qt::black));
                item->setBrush(QBrush(Qt::darkRed));
                item->partsCnt = partI;
                item->clusterNum = cluster;
                partsScene.addItem(item);
                i++;
            }
        }

        if (partI >= 1) {
            const QMap<int, Cluster*> *clustersPrev = &clustersVector.at(partI-1);

            int distanceElementsPrevCntMax = 0;
            foreach (int clusterPrev, clustersPrev->keys()) {
                int elementsCount = clustersPrev->value(clusterPrev)->getElementsCount();
                if (elementsCount > distanceElementsPrevCntMax)
                    distanceElementsPrevCntMax = elementsCount;
            }

            foreach (int clusterPrev, clustersPrev->keys()) {
                // номер кластера, в который попали distElementOld и количество попавших туда элементов
                QMap<int, int> linesCount;

                // пробегаемся по всем элементам текущего кластера
                int distanceElementsPrevCnt = clustersPrev->value(clusterPrev)->getElementsCount();
                for (int distElementPrev = 0; distElementPrev < distanceElementsPrevCnt; distElementPrev++) {
                    const DistanceElement *distElementTmp = clustersPrev->value(clusterPrev)->getDistanceElement(distElementPrev);

                    // ищем этот элемент в кластерах clustersI
                    foreach (int cluster, clusters->keys()) {
                        int distanceElementsCnt = clusters->value(cluster)->getElementsCount();
                        bool isFinished = false;
                        for (int distElementI = 0; distElementI < distanceElementsCnt; distElementI++) {
                            if (distElementTmp == clusters->value(cluster)->getDistanceElement(distElementI)) {
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
                    // рисуем только линии, которые описывают переход более 5% участков
                    if (((ui->fivePercentLinesLimitBox->isChecked()) &&
                         ((double)linesCount.value(line)/(double)maxClusterElements > 0.05)) ||
                            (!ui->fivePercentLinesLimitBox->isChecked()))
                    {
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
}

void SplitsClusterisationTune::on_clustersTable_cellClicked(int row, int column)
{
    if (column <= 1) return;

    int tsNum = column - 2;
    int clusterNum = ui->clustersTable->item(row, 0)->text().toInt();

    QVector<const DistanceElement*> distanceElements = splitsClusterisation->getClusters()->value(clusterNum)->getDistanceElements();
    QVector<int> parts;
    for (int i = 0; i < distanceElements.size(); i++) {
        if (distanceElements.at(i)->getTsNum() == tsNum) parts.append(distanceElements.at(i)->getWindowNum());
    }

    splitsClusterisation->getDataKeepers()->at(tsNum)->showParts(parts);
}

void SplitsClusterisationTune::updateClustersDistributionTable()
{
    int tsCount = splitsClusterisation->getDataKeepers()->size();
    QMap<int, Cluster*> *clusters = splitsClusterisation->getClusters();
    QMap<int, QString> clustersColors;

    QStringList colorNames = QColor::colorNames();
    int k = 0;
    foreach (int cluster, clusters->keys()) {
        clustersColors[cluster] = colorNames.at((k+10)%colorNames.size());
        k++;
    }

    ui->clusterDistributionTable->setRowCount(tsCount);

    for (int i = 0; i < tsCount; i++) {
        QWidget *w = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        w->setLayout(layout);

        foreach (int cluster, clusters->keys()) {
            int windowsCount = splitsClusterisation->getDataKeepers()->at(i)->getWindowsCount();
            QVector<int> partsDistribution = clusters->value(cluster)->getPartsDistribution();
            int partsCount = partsDistribution.at(i);

            if (partsCount > 0) {
                int percentageRatio = 100*partsCount/windowsCount;

                QWidget *tmpW = new QWidget;
                tmpW->setStyleSheet(QString("background: %1").arg(clustersColors[cluster]));

                if (percentageRatio > 20) {
                    QLabel *tmpLabel = new QLabel(QString("  %1").arg(percentageRatio));
                    tmpLabel->setStyleSheet(QString("color: white"));
                    QHBoxLayout *tmpL = new QHBoxLayout();
                    tmpL->setMargin(0);
                    tmpL->setSpacing(0);
                    tmpL->addWidget(tmpLabel);
                    //tmpL->setStretchFactor(tmpLabel, percentageRatio);

                    tmpW->setLayout(tmpL);
                }

                layout->addWidget(tmpW);
                layout->setStretchFactor(tmpW, percentageRatio);
            }
        }

        QString name = QFileInfo(splitsClusterisation->getDataKeepers()->at(i)->getDataFileName()).baseName();
        QTableWidgetItem *item1 = new QTableWidgetItem(name);
        item1->setToolTip(splitsClusterisation->getDataKeepers()->at(i)->getDataFileName());
        ui->clusterDistributionTable->setItem(i, 0, item1);

        ui->clusterDistributionTable->setCellWidget(i, 1, w);
    }

    ui->clusterDistributionTable->resizeColumnsToContents();
}
