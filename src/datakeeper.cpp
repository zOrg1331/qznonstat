#include "datakeeper.h"

#include "zchart.h"

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
