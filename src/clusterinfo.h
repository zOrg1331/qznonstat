#ifndef CLUSTERINFO_H
#define CLUSTERINFO_H

#include <QWidget>

class ZChart;
class ZSimpleSeries;

class DataKeeper;
class Cluster;

namespace Ui {
    class ClusterInfo;
}

class ClusterInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ClusterInfo(QWidget *parent = 0);
    ~ClusterInfo();

    void showClusterInfo(const QList<DataKeeper *> *dataKeepers, QMap<int, Cluster*> *clusters, int clusterNum);
    
private:
    Ui::ClusterInfo *ui;

    ZChart *chart;
    QList<ZSimpleSeries *> clustersSeries;

};

#endif // CLUSTERINFO_H
