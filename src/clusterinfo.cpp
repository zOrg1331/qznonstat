#include "clusterinfo.h"
#include "ui_clusterinfo.h"

#include "zchart.h"
#include "datakeeper.h"
#include "distanceelement.h"
#include "cluster.h"

ClusterInfo::ClusterInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClusterInfo)
{
    ui->setupUi(this);
    
    chart = new ZChart();
    chart->setBorderType(0);
    chart->setGraphPanelBorderType(0);
    chart->setMargin(2);
    chart->setSpacing(2);
    chart->setFirstText("");
    chart->setSecondText(" ");
    chart->setTitle(trUtf8("временной ряд"));
    chart->setXRange(0, 1000);
    chart->setYRange(0, 10);
    chart->setAxesName("", "");
    chart->showLegend(true);
    chart->setAutoscale(true);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(chart);
    
    ui->clustersGraphBox->setLayout(layout);
    
    this->setWindowFlags(Qt::Window);
}

ClusterInfo::~ClusterInfo()
{
    delete ui;
    delete chart;
    foreach (ZSimpleSeries *clusterData, clustersSeries) {
        delete clusterData;
    }
}

void ClusterInfo::showClusterInfo(const QList<DataKeeper *> *dataKeepers, QMap<int, Cluster*> *clusters, int clusterNum)
{
    chart->removeSeries();
    foreach (ZSimpleSeries *clusterData, clustersSeries) {
        delete clusterData;
    }
    clustersSeries.clear();

    int tsCount = dataKeepers->count();
    int xPos = 0;
    
    QStringList colorNames = QColor::colorNames();

    for (int tsNum = 0; tsNum < tsCount; tsNum++) {
        QVector<const DistanceElement*> distanceElements = clusters->value(clusterNum)->getDistanceElements();
        QVector<int> parts;
        for (int i = 0; i < distanceElements.size(); i++) {
            if (distanceElements.at(i)->getTsNum() == tsNum) parts.append(distanceElements.at(i)->getWindowNum());
        }
        
        qSort(parts);
        
        ZSimpleSeries *clusterData = new ZSimpleSeries(dataKeepers->at(tsNum)->getDataFileNameShort());
        clusterData->setColor(QColor(colorNames.at(tsNum+10)));
        chart->addSeries(clusterData);
        clustersSeries.append(clusterData);
        
        clusterData->add(xPos, 0);
        
        for (int i = 0; i < parts.size(); i++) {
            QVector<double> data;
            dataKeepers->at(tsNum)->getDataInWindow(i, &data);
            for (int j = 0; j < data.size(); j++) {
                clusterData->add(xPos, data.at(j));
                xPos++;
            }
        }
    }
    
    chart->autoscale1(0, xPos);
    chart->setXRange(0, xPos);
    chart->update();
}
