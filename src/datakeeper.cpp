#include "datakeeper.h"

#include "zchart.h"
#include "common_math_tools.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

DataKeeper::DataKeeper()
{
    dataFrom = 0;
    dataTo = 10000;
    dataWindow = 500;
    dataWindowStep = 500;
    dataUseFullTs = true;
    dataUseSeconds = false;
    dataSampling = 250;
    dataNormalize = true;

    mean = 0.0;
    disp = 1.0;
    stddev = 1.0;

    chart = new ZChart();
    chart->setBorderType(0);
    chart->setGraphPanelBorderType(0);
    chart->setMargin(2);
    chart->setSpacing(2);
    chart->setFirstText("");
    chart->setSecondText(" ");
    chart->setTitle(trUtf8("временной ряд"));
    chart->setXRange(dataFrom, dataTo);
    chart->setYRange(0, 10);
    chart->setAxesName("", "");
    chart->showLegend(false);
    chart->setAutoscale(true);

    graph = new ZSimpleSeries("");
    graph->setColor(Qt::red);
    chart->addSeries(graph);

    clusterGraph = new ZSimpleSeries("");
    clusterGraph->setColor(Qt::darkGreen);
    chart->addSeries(clusterGraph);
}

DataKeeper::~DataKeeper()
{
    delete graph;
    delete clusterGraph;
    delete chart;
}

void DataKeeper::setDataUseFullTs(const bool useFullTs)
{
    dataUseFullTs = useFullTs;
    if (useFullTs) {
        dataFrom = 0;
        dataTo = data.size();
    }
}

QWidget *DataKeeper::getWidget()
{
    return  qobject_cast<QWidget*>(chart);
}

void DataKeeper::setDataFileName(const QString name)
{
    fileName = name;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << QString("cannot open file: %1\n").arg(fileName);
        return;
    }

    QString str;
    bool ok;
    int line = 0;
    double currNum;
    data.clear();
    while (!file.atEnd()) {
        str = file.readLine();
        str = str.split(' ').last();
        currNum = str.toDouble(&ok);
        if (!ok) {
            qDebug() << QString("garbage at line %1 in file: %2\n")
                    .arg(line).arg(fileName);
            return;
        }
        data.append(currNum);
        line++;
    }

    file.close();

    if (dataUseFullTs) {
        dataFrom = 0;
        dataTo = data.size();
    }

    redrawData();

    CommonMathTools::calcStats(data, &mean, &disp, &stddev);
}

void DataKeeper::redrawData()
{
    int sampling = 1;
    graph->clear();
    if (dataUseSeconds) sampling = dataSampling;
    for (int i = 0; i < data.size(); i++) {
        graph->add(i/(double)sampling, data.at(i));
    }
    if (!dataUseFullTs) {
        chart->setXRange(dataFrom, dataTo);
        chart->autoscale1(dataFrom, dataTo);
    } else {
        chart->autoscale();
    }
}

QString DataKeeper::getDescription()
{
    return fileName.isEmpty() ? trUtf8("ряд: %1").arg(num+1) : QFileInfo(fileName).baseName();
}

int DataKeeper::getWindowsCount()
{
    return (dataTo-dataFrom)/dataWindowStep - dataWindow/dataWindowStep;
}

void DataKeeper::getDataInWindow(int windowNum, QVector<double> *dataIn)
{
    double val;
    dataIn->clear();
    for (int i = 0; i < dataWindow; i++) {
        val = data.at(dataFrom + windowNum*dataWindowStep + i);
        if (dataNormalize) {
            val -= mean;
            val /= stddev;
        }
        dataIn->append(val);
    }
}

void DataKeeper::showParts(const QVector<int> &parts_)
{
    double clusterLength = dataWindowStep;

    QVector<int> parts = parts_;
    qSort(parts);

    double min = 0;
    double max = 0;
    if (parts.size() > 0) {
        min = graph->getMinYy(dataFrom + dataWindowStep*parts.first(), dataFrom + dataWindowStep*parts.last() + dataWindow);
        max = graph->getMaxYy(dataFrom + dataWindowStep*parts.first(), dataFrom + dataWindowStep*parts.last() + dataWindow);
    }

    clusterGraph->clear();

    for (int i = 0; i < parts.size(); i++) {
        clusterGraph->add(dataFrom + dataWindowStep*parts.at(i), min);
        clusterGraph->add(dataFrom + dataWindowStep*parts.at(i), max);
        clusterGraph->add(dataFrom + dataWindowStep*parts.at(i)+clusterLength, max);
        clusterGraph->add(dataFrom + dataWindowStep*parts.at(i)+clusterLength, min);
    }

    redrawData();
}
