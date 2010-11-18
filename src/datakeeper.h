#ifndef DATAKEEPER_H
#define DATAKEEPER_H

#include <QWidget>
#include <QString>

class ZChart;
class ZSimpleSeries;

class DataKeeper : public QObject
{
    Q_OBJECT
public:
    explicit DataKeeper();
    ~DataKeeper();

    void setNum(const int n) { num = n; }

    void setDataFileName(const QString fileName);
    QString getDataFileName() { return fileName; }

    void setDataFrom(const int from) { dataFrom = from; }
    void setDataTo(const int to) { dataTo = to; }
    void setDataWindow(const int window) { dataWindow = window; }
    void setDataWindowStep(const int windowStep) { dataWindowStep = windowStep; }
    void setDataUseFullTs(const bool useFullTs);
    void setDataUseSeconds(const bool useSeconds) { dataUseSeconds = useSeconds; }
    void setDataSampling(const int sampling) { dataSampling = sampling; }
    void setDataNormalize(const bool normalize) { dataNormalize = normalize; }

    int getDataFrom() { return dataFrom; }
    int getDataTo() { return dataTo; }
    int getDataWindow() { return dataWindow; }
    int getDataWindowStep() { return dataWindowStep; }
    bool getDataUseFullTs() { return dataUseFullTs; }
    bool getDataUseSeconds() { return dataUseSeconds; }
    int getDataSampling() { return dataSampling; }
    bool getDataNormalize() { return dataNormalize; }

    int getWindowsCount();

    void getDataInWindow(int windowNum, QVector<double> *dataIn);

    QWidget *getWidget();
    QString getDescription();

    void redrawData();
    
    void showParts(const QVector<int> &parts);

signals:

public slots:

private:
    ZChart *chart;
    ZSimpleSeries *graph;
    ZSimpleSeries *clusterGraph;

    int num;
    QString fileName;
    QVector<double> data;

    int dataFrom;
    int dataTo;
    int dataWindow;
    int dataWindowStep;
    bool dataUseFullTs;
    bool dataUseSeconds;
    int dataSampling;
    bool dataNormalize;

    double mean;
    double disp;
    double stddev;

};

#endif // DATAKEEPER_H
