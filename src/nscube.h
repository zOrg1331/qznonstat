#ifndef NSCUBE_H
#define NSCUBE_H

#include <QVector>

class DistanceElement;

class NSCube
{
public:
    NSCube()
    {
        init();
    }
    
    NSCube(const NSCube &other)
    {
        init();
        setCoords(other.coords);
        setDistanceElements(other.distanceElements);
    }
    
    NSCube& operator=(const NSCube &other)
    {
        init();
        setCoords(other.coords);
        setDistanceElements(other.distanceElements);
        return *this;
    }
    
    const QVector<int> & getCoords() const
    {
        return coords;
    }
    
    void setCoords(const QVector<int> &coords_)
    {
        coords.resize(0);
        for (int i = 0; i < coords_.size(); i++) {
            coords << coords_.at(i);
        }
    }
    
    const QVector<const DistanceElement*> & getDistanceElements() const
    {
        return distanceElements;
    }
    
    void setDistanceElements(const QVector<DistanceElement> &distanceElements_)
    {
        distanceElements.resize(0);
        for (int i = 0; i < distanceElements_.size(); i++) {
            distanceElements << &(distanceElements_.at(i));
        }
    }
    
    void setDistanceElements(const QVector<const DistanceElement*> &distanceElements_)
    {
        distanceElements.resize(0);
        for (int i = 0; i < distanceElements_.size(); i++) {
            distanceElements << distanceElements_.at(i);
        }
    }
    
    void appendDistanceElement(const DistanceElement &distanceElement_)
    {
        distanceElements.append(&distanceElement_);
    }
    
    int isInCluster()
    {
        return inCluster;
    }
    
    void setInCluster(int inCluster_)
    {
        inCluster = inCluster_;
    }
    
    bool isNeighbors(const NSCube *other)
    {
        bool zflag = false;
        for (int i = 0; i < coords.size(); i++) {
            int dist = coords.at(i) - other->getCoords().at(i);
            if (abs(dist) > 1) return false;
            if (abs(dist) == 1) {
                if (zflag) {
                    // только по одной координате допускается расстояние 1
                    // по остальным должно быть 0 (исключаются соприкасающиеся углами кубики)
                    return false;
                } else {
                    zflag = true;
                }
            }
        }
        return true;
    }
    
private:
    void init()
    {
        inCluster = -1;
        distanceElements.resize(0);
    }
    
    QVector<int> coords;
    
    QVector<const DistanceElement *> distanceElements;
    
    int inCluster;
    
};

#endif // NSCUBE_H
