#ifndef ARDATAANALYSIS_H
#define ARDATAANALYSIS_H

#include <QThread>

class DataKeeper;
class CommonMathTools;
class DistanceElement;

class ArDataAnalysis : public QThread
{
    Q_OBJECT
public:
    explicit ArDataAnalysis(QObject *parent = 0);

    void setDataKeepers(const QList<DataKeeper *> *keepers) { dataKeepers = keepers; }
    void setDistanceElements(QVector<DistanceElement> *dElements) { distanceElements = dElements; }
    QVector<DistanceElement> *getDistanceElements() { return distanceElements; }

    int getEstimatedTime();

    void setParams(int dim, int ord) { dimension = dim; order = ord; }

    int getDimension() { return dimension; }
    int getOrder() { return order; }

protected:
    void run();

signals:
    void progressStep(int);

public slots:

private:
    void calc();

    const QList<DataKeeper *> *dataKeepers;
    QVector<DistanceElement> *distanceElements;
    CommonMathTools *cmtObj;
    int dimension;
    int order;

    int progress;
};

#endif // ARDATAANALYSIS_H
