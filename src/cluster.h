#ifndef CLUSTER_H
#define CLUSTER_H

#include <QVector>
#include <QMap>

#include <QtDebug>

#include "distanceelement.h"

class Cluster
{
public:
    Cluster()
    {
        init();
    }

    ~Cluster()
    {
        init();
    }

    Cluster(const Cluster &other)
    {
        init();
        setDistanceElements(other.distanceElements);
    }

    Cluster(const QVector<DistanceElement> &distanceElements)
    {
        init();
        setDistanceElements(distanceElements);
    }

    Cluster& operator=(const Cluster &other)
    {
        init();
        setDistanceElements(other.distanceElements);
        return *this;
    }

    bool operator==(const Cluster &other)
    {
        if (distanceElements == other.distanceElements) return true;
        else return false;
    }
    
    static bool compare(const Cluster *c1, const Cluster *c2)
    {
        QVector<int> partsD1 = c1->getPartsDistribution();
        QVector<int> partsD2 = c2->getPartsDistribution();
        if (partsD1.size() != partsD2.size()) return false;
        int k = 0;
        for (int i = 0; i < partsD1.size(); i++) {
            if (partsD1.at(i) > partsD2.at(i)) k++;
            else k--;
        }
        if (k >= 0) return false;
        return true;
    }
    
    void setNum(int num_)
    {
        num = num_;
    }
    
    int getNum() const
    {
        return num;
    }
    
    void setTsCount(int tsCount_)
    {
        tsCount = tsCount_;
    }

    const QVector<const DistanceElement*> & getDistanceElements() const
    {
        return distanceElements;
    }

    void setDistanceElements(const QVector<DistanceElement> &distanceElements_)
    {
        init();
        for (int i = 0; i < distanceElements_.size(); i++) {
            distanceElements.append(&(distanceElements_.at(i)));
        }
    }

    void setDistanceElements(const QVector<const DistanceElement*> &distanceElements_)
    {
        init();
        for (int i = 0; i < distanceElements_.size(); i++) {
            distanceElements.append(distanceElements_.at(i));
        }
    }

    void appendDistanceElements(const QVector<DistanceElement> &distanceElements_)
    {
        for (int i = 0; i < distanceElements_.size(); i++) {
            distanceElements.append(&(distanceElements_.at(i)));
        }
    }
    
    void appendDistanceElements(const QVector<const DistanceElement*> &distanceElements_)
    {
        for (int i = 0; i < distanceElements_.size(); i++) {
            distanceElements.append(distanceElements_.at(i));
        }
    }

    int getElementsCount() const
    {
        return distanceElements.size();
    }

    const DistanceElement *getDistanceElement(int num) const
    {
        return distanceElements.at(num);
    }
    
    QVector<int> getPartsDistribution() const
    {
        QVector<int> res;
        res.resize(tsCount);
        res.fill(0);
        for (int i = 0; i < distanceElements.size(); i++) {
            res[distanceElements.at(i)->getTsNum()]++;
        }
        return res;
    }

private:
    void init();

    QVector<const DistanceElement*> distanceElements;
    
    int num;
    int tsCount;
};

#endif // Cluster_H
