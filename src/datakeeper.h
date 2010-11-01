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

    void setNum(int n) { num = n; }
    QWidget *getWidget();
    QString getDescription();
    void setDataFileName(const QString fileName);
    void redrawData();

signals:

public slots:

private:
    ZChart *chart;
    ZSimpleSeries *graph;

    int num;
    QString fileName;
    QVector<double> data;
};

#endif // DATAKEEPER_H
