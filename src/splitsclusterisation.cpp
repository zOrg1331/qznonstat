#include "splitsclusterisation.h"

#include <boost/math/tools/stats.hpp>
#include <math.h>

#include "datakeeper.h"
#include "distanceelement.h"
#include "nscube.h"
#include "cluster.h"

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
    int m = 1 << splitsCount;
    int d = distanceElements->at(0).getCoeffsCount();
    double max = -1e10;
    double min = 1e10;

    for (int i = 0; i < distanceElements->size(); i++) {
        QVector<double> coeffs = distanceElements->at(i).getCoeffs();
        for (int j = 0; j < d; j++) {
            if (coeffs.at(j) > max) max = coeffs.at(j);
            if (coeffs.at(j) < min) min = coeffs.at(j);
        }
    }
    max += 0.01*max;
    min -= 0.01*min;

    QVector<NSCube *> hyperCube;

    for (int i = 0; i < distanceElements->size(); i++) {
        QVector<int> cubeCoord = distanceElements->at(i).getCubeCoord(m, min, max);
        for (int j = 0; j <= hyperCube.size(); j++) {
            if (j == hyperCube.size()) {
                NSCube *cube = new NSCube;
                cube->setCoords(cubeCoord);
                cube->appendDistanceElement(distanceElements->at(i));
                hyperCube.append(cube);
                break;
            }
            if (hyperCube.at(j)->getCoords() == cubeCoord) {
                hyperCube.at(j)->appendDistanceElement(distanceElements->at(i));
                break;
            }
        }
    }

    // проводим кластеризацию
    // метод взят из
    // Мандель И.Д. "Кластерный анализ", стр. 56
    for (int i = 0, cluster = 0; i < hyperCube.size(); i++) {
        if (markCubeAndNeighborsAsCluster(hyperCube, i, cluster)) cluster++;
    }

    // заполняем карту кластеров
    foreach (int cluster, clusters->keys()) {
        delete clusters->take(cluster);
    }
    clusters->clear();

    for (int i = 0; i < hyperCube.size(); i++) {
        int cluster = hyperCube.at(i)->isInCluster();
        if (cluster >= 0) {
            if (clusters->contains(cluster)) {
                clusters->value(cluster)->appendDistanceElements(hyperCube.at(i)->getDistanceElements());
            } else {
                clusters->insert(cluster,
                                 new Cluster(hyperCube.at(i)->getDistanceElements()));
            }
        }
    }

    for (int i = 0; i < hyperCube.size(); i++) {
        delete hyperCube[i];
    }
    hyperCube.resize(0);
}

bool SplitsClusterisation::markCubeAndNeighborsAsCluster(const QVector<NSCube *> & hyperCube,
                                                         int cubeNum,
                                                         int clusterNum)
{
    // входит ли этот кубик уже в какой-то кластер?
    // есть ли элементы в этом кубике?
    if ((hyperCube.at(cubeNum)->isInCluster() == -1)
        && (hyperCube.at(cubeNum)->getDistanceElements().size() > 0)) {
        // помечаем этот кубик, как входящий в кластер
        hyperCube.at(cubeNum)->setInCluster(clusterNum);
        // помечаем всех его соседей и соседей их соседей, как входящих в этот кластер
        for (int j = 0; j < hyperCube.size(); j++) {
            if ((cubeNum != j) && (hyperCube.at(cubeNum)->isNeighbors(hyperCube.at(j)))) {
                // это наш сосед
                markCubeAndNeighborsAsCluster(hyperCube, j, clusterNum);
            }
        }
        return true;
    }
    return false;
}
