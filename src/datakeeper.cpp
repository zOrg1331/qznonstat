#include "datakeeper.h"

#include "zchart.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

DataKeeper::DataKeeper()
{
    chart = new ZChart();
    chart->setBorderType(0);
    chart->setGraphPanelBorderType(0);
    chart->setMargin(2);
    chart->setSpacing(2);
    chart->setFirstText("");
    chart->setSecondText(" ");
    chart->setTitle(trUtf8("временной ряд"));
    chart->setXRange(0, 10);
    chart->setYRange(-10, 10);
    chart->setAxesName("", "");
    chart->showLegend(false);
    chart->setAutoscale(true);

    graph = new ZSimpleSeries("");
    graph->setColor(Qt::red);
    chart->addSeries(graph);
}

DataKeeper::~DataKeeper()
{
    delete graph;
    delete chart;
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

    redrawData();
}

void DataKeeper::redrawData()
{
    graph->clear();
    for (int i = 0; i < data.size(); i++) {
        graph->add(i, data.at(i));
    }
    chart->autoscale();
}

QString DataKeeper::getDescription()
{
    return fileName.isEmpty() ? trUtf8("ряд: %1").arg(num+1) : QFileInfo(fileName).baseName();
}
