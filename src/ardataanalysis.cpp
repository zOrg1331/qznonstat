#include "ardataanalysis.h"

#include <boost/math/special_functions/factorials.hpp>
#include <boost/math/tools/stats.hpp>
#include <math.h>

#include "common_math_tools.h"
#include "datakeeper.h"

ArDataAnalysis::ArDataAnalysis(QObject *parent) :
    QThread(parent)
{
    cmtObj = new CommonMathTools();
}

int ArDataAnalysis::getEstimatedTime()
{
    int res = 0;
    for (int ts = 0; ts < dataKeepers->size(); ts++) {
        res += dataKeepers->at(ts)->getWindowsCount();
    }
    return res;
}

void ArDataAnalysis::run()
{
    calc();
}

void ArDataAnalysis::calc()
{
    progress = 0;

    QVector<double> data;
    for (int ts = 0; ts < dataKeepers->size(); ts++) {
        for (int w = 0; w < dataKeepers->at(ts)->getWindowsCount(); w++) {
            dataKeepers->at(ts)->getDataInWindow(w, &data);

            VECTOR_D dataV;
            dataV.resize(data.size());
            for (int i = 0; i < data.size(); i++) {
                dataV[i] = data.at(i);
            }

            VECTOR_D arCoeffs;
            arCoeffs.resize(dataKeepers->at(ts)->getWindowsCount());

            cmtObj->lls_solve_single_ts(dimension, order, dataV, &arCoeffs);

            progress++;
            emit progressStep(progress);
        }
    }
}
