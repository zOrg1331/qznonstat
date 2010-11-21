#ifndef ARMODELTUNE_H
#define ARMODELTUNE_H

#include <QtGui>

#include "ardataanalysis.h"

class ZChart;
class ZSimpleSeries;

namespace Ui {
    class ArModelTune;
}

class ArModelTune : public QWidget {
    Q_OBJECT
public:
    ArModelTune(ArDataAnalysis *arDataAnalysis, QWidget *parent = 0);
    ~ArModelTune();

protected:

private:
    void showModel();
    void updateGraphs(bool normalize);

    Ui::ArModelTune *ui;

    ArDataAnalysis *arDataAnalysis;

    ZChart *coeffsChart;
    ZChart *swartzChart;
    QVector<ZSimpleSeries *> coeffsSeries;
    ZSimpleSeries * swartzSeries;

    QProgressDialog *progress;

    QVector<QCheckBox *> coeffsCheckBoxes;

    QVector<int> coeffsSelection;

private slots:
    void on_calcOptimumDimension_clicked();
    void on_normalizeCoeffsChk_clicked();
    void on_updateGraphsButton_clicked();

    void updateCoeffsSelection();

signals:
    void progressStep(int);
};

#endif // ArModelTune_H
