#include "ardataanalysis.h"

ArDataAnalysis::ArDataAnalysis(QObject *parent) :
    QThread(parent)
{
}

int ArDataAnalysis::getEstimatedTime()
{
    return 10;
}

void ArDataAnalysis::run()
{
    static int t = 0;
    while (t <= 10) {
        emit progressStep(++t);
        sleep(1);
    }
}
