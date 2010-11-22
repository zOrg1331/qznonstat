#ifndef CLUSTER_H
#define CLUSTER_H

#include <QVector>

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

    int getElementsCount() const
    {
        return distanceElements.size();
    }

    const DistanceElement *getDistanceElement(int num) const
    {
        return distanceElements.at(num);
    }

private:
    void init();

    QVector<const DistanceElement*> distanceElements;
};

#endif // Cluster_H
