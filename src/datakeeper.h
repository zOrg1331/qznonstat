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
    void setDataUseFullTs(const bool useFullTs) { dataUseFullTs = useFullTs; }
    void setDataUseSeconds(const bool useSeconds) { dataUseSeconds = useSeconds; }
    void setDataSampling(const int sampling) { dataSampling = sampling; }

    int getDataFrom() { return dataFrom; }
    int getDataTo() { return dataTo; }
    int getDataWindow() { return dataWindow; }
    int getDataWindowStep() { return dataWindowStep; }
    bool getDataUseFullTs() { return dataUseFullTs; }
    bool getDataUseSeconds() { return dataUseSeconds; }
    int getDataSampling() { return dataSampling; }

    QWidget *getWidget();
    QString getDescription();

    void redrawData();

signals:

public slots:

private:
    ZChart *chart;
    ZSimpleSeries *graph;

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

};

#endif // DATAKEEPER_H
