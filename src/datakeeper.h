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

    void setNum(int n) { num = n; }
    QWidget *getWidget();
    QString getDescription() { return trUtf8("ряд: %1").arg(num+1); }

signals:

public slots:

private:
    ZChart *chart;
    ZSimpleSeries *graph;

    int num;
};

#endif // DATAKEEPER_H
