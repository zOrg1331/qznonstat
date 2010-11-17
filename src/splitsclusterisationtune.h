#ifndef SPLITSCLUSTERISATIONTUNE_H
#define SPLITSCLUSTERISATIONTUNE_H

#include <QtGui>

#include "splitsclusterisation.h"

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
    
    QVector<QMap<int, QVector<DistanceElement> > *> clustersVector;

private slots:
    void updateClustersTable();
    void updateClustersGraph();
    void on_calcClustersButton_clicked();
    void on_drawClustersGraph_clicked();
    
signals:
    void progressStep(int);
};

#endif // SPLITSCLUSTERISATIONTUNE_H
