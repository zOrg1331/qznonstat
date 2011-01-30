#ifndef SPLITSCLUSTERISATIONTUNE_H
#define SPLITSCLUSTERISATIONTUNE_H

#include <QtGui>

#include "splitsclusterisation.h"

class ClusterInfo;

namespace Ui {
    class SplitsClusterisationTune;
}

class SplitsClusterisationTune : public QWidget {
    Q_OBJECT
public:
    SplitsClusterisationTune(SplitsClusterisation *splitsClusterisation, QWidget *parent = 0);
    ~SplitsClusterisationTune();

protected:

private:

    Ui::SplitsClusterisationTune *ui;

    SplitsClusterisation *splitsClusterisation;

    QProgressDialog *progress;

    QGraphicsScene partsScene;

    QVector<QMap<int, Cluster> > clustersVector;
    
    ClusterInfo *clusterInfo;

private slots:
    void updateClustersTable();
    void updateClustersDistributionTable();
    void updateClustersGraph();
    void on_calcClustersButton_clicked();
    void on_drawClustersGraph_clicked();
    void on_clustersTable_cellClicked(int row, int column);
    void showTsOfCurrentCluster();
    
signals:
    void progressStep(int);
};

#endif // SPLITSCLUSTERISATIONTUNE_H
