#ifndef SPLITSCLUSTERISATION_H
#define SPLITSCLUSTERISATION_H

#include <QThread>
#include <QMap>

class QString;

class DataKeeper;
class DistanceElement;
class Cluster;
class NSCube;

class SplitsClusterisation : public QThread
{
    Q_OBJECT
public:
    explicit SplitsClusterisation(QObject *parent = 0);

    void setDataKeepers(const QList<DataKeeper *> *keepers) { dataKeepers = keepers; }
    const QList<DataKeeper *> *getDataKeepers() { return dataKeepers; }

    void setDistanceElements(const QVector<DistanceElement> *dElements) { distanceElements = dElements; }
    const QVector<DistanceElement> *getDistanceElements() { return distanceElements; }

    void setClusters(QMap<int, Cluster*> *clusters_) { clusters = clusters_; }
    QMap<int, Cluster*> *getClusters() { return clusters; }

    int getEstimatedTime();

    void setParams(int splits, int minparts) { splitsCount = splits; minPartsCount = minparts; }

    int getSplits() { return splitsCount; }
    int getMinParts() { return minPartsCount; }
    
    QString getClustersReport();

protected:
    void run();

signals:
    void progressStep(int);

public slots:

private:
    void calc();
    bool markCubeAndNeighborsAsCluster(const QVector<NSCube *> & hyperCube,
                                       int cubeNum,
                                       int clusterNum);

    const QList<DataKeeper *> *dataKeepers;
    const QVector<DistanceElement> *distanceElements;
    QMap<int, Cluster*> *clusters;

    int splitsCount;
    int minPartsCount;

    int progress;
};

#endif // SPLITSCLUSTERISATION_H
