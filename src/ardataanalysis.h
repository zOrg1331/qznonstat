#ifndef ARDATAANALYSIS_H
#define ARDATAANALYSIS_H

#include <QThread>

class DataKeeper;
class CommonMathTools;

class ArDataAnalysis : public QThread
{
    Q_OBJECT
public:
    explicit ArDataAnalysis(QObject *parent = 0);

    void setDataKeepers(const QList<DataKeeper *> *keepers) { dataKeepers = keepers; }

    int getEstimatedTime();

    void setParams(int dim, int ord) { dimension = dim; order = ord; }

protected:
    void run();

signals:
    void progressStep(int);

public slots:

private:
    void calc();

    const QList<DataKeeper *> *dataKeepers;
    CommonMathTools *cmtObj;
    int dimension;
    int order;

    int progress;
};

#endif // ARDATAANALYSIS_H
