#ifndef SPLITSCLUSTERISATION_H
#define SPLITSCLUSTERISATION_H

#include <QThread>

class DataKeeper;
class DistanceElement;

class SplitsClusterisation : public QThread
{
    Q_OBJECT
public:
    explicit SplitsClusterisation(QObject *parent = 0);

    void setDataKeepers(const QList<DataKeeper *> *keepers) { dataKeepers = keepers; }
    void setDistanceElements(QVector<DistanceElement> *dElements) { distanceElements = dElements; }
    QVector<DistanceElement> *getDistanceElements() { return distanceElements; }

    int getEstimatedTime();

    void setParams(int splits, int minparts) { splitsCount = splits; minPartsCount = minparts; }

    int getSplits() { return splitsCount; }
    int getMinParts() { return minPartsCount; }

protected:
    void run();

signals:
    void progressStep(int);

public slots:

private:
    void calc();

    const QList<DataKeeper *> *dataKeepers;
    QVector<DistanceElement> *distanceElements;
    int splitsCount;
    int minPartsCount;

    int progress;
};

#endif // SPLITSCLUSTERISATION_H
