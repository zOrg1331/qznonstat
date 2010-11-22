#include "cluster.h"

void Cluster::init()
{
    // осознанно не удаляем объекты, т.к. это ссылки на объекты из более глобального вектора
//    for (int i = 0; i < distanceElements.size(); i++) {
//        delete distanceElements[i];
//    }
    distanceElements.resize(0);
}
