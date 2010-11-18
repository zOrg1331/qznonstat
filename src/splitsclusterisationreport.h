#ifndef SPLITSCLUSTERISATIONREPORT_H
#define SPLITSCLUSTERISATIONREPORT_H

#include <QtGui>

#include "splitsclusterisation.h"

namespace Ui {
    class SplitsClusterisationReport;
}

class SplitsClusterisationReport : public QWidget {
    Q_OBJECT
public:
    SplitsClusterisationReport(SplitsClusterisation *splitsClusterisation, QWidget *parent = 0);
    ~SplitsClusterisationReport();

private:
    Ui::SplitsClusterisationReport *ui;
    
    SplitsClusterisation *splitsClusterisation;

private slots:

    void on_updateRepButton_clicked();
    
    void on_saveRepButton_clicked();
    
signals:

};

#endif
