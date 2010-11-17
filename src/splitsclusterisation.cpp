#include "splitsclusterisation.h"

#include <boost/math/tools/stats.hpp>
#include <math.h>

#include "datakeeper.h"
#include "distanceelement.h"

SplitsClusterisation::SplitsClusterisation(QObject *parent) :
    QThread(parent)
{
}

int SplitsClusterisation::getEstimatedTime()
{
    int res = 0;
    for (int ts = 0; ts < dataKeepers->size(); ts++) {
        res += dataKeepers->at(ts)->getWindowsCount();
    }
    return res;
}

void SplitsClusterisation::run()
{
    calc();
}

void SplitsClusterisation::calc()
{
}
