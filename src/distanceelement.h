#ifndef DISTANCEELEMENT_H
#define DISTANCEELEMENT_H

#include <QVector>
#include <qglobal.h>

#include <math.h>

class DistanceElement
{
public:
    DistanceElement()
    {
        init();
    }

    DistanceElement(const DistanceElement &other)
    {
        init();
        tsNum = other.tsNum;
        windowNum = other.windowNum;
        coeffs = other.coeffs;
    }

    DistanceElement& operator=(const DistanceElement &other)
    {
        init();
        tsNum = other.tsNum;
        windowNum = other.windowNum;
        coeffs = other.coeffs;
        return *this;
    }

    bool operator==(const DistanceElement &other)
    {
        if (coeffs == other.coeffs) return true;
        else return false;
    }

    int getTsNum() const { return tsNum; }

    void setTsNum(int tsNum_) { tsNum = tsNum_; }

    int getWindowNum() const { return windowNum; }

    void setWindowNum(int windowNum_) { windowNum = windowNum_; }

    const QVector<double> & getCoeffs() const
    {
        return coeffs;
    }

    void setCoeffs(const QVector<double> &coeffs_)
    {
        coeffs.resize(0);
        for (int i = 0; i < coeffs_.size(); i++) {
            coeffs << coeffs_.at(i);
        }
    }

    int getCoeffsCount() const
    {
        return coeffs.size();
    }

    double getCoeffsVectorLength(void)
    {
        if (coeffsVectorLength == -1) {
            double length = 0;
            for (int coeff = 0; coeff < coeffs.size(); coeff++) {
                length += coeffs.at(coeff)*coeffs.at(coeff);
            }
            coeffsVectorLength = sqrt(length);
            return coeffsVectorLength;
        }
        return coeffsVectorLength;
    }

    QVector<int> getCubeCoord(int partsCnt, double min, double max) const
    {
        QVector<int> res;
        for (int i = 0; i < coeffs.size(); i++) {
            res << coeffs.at(i)/((fabs(max)+fabs(min))/partsCnt);
        }
        return res;
    }

    double getDistance(const DistanceElement &other,
                       const QVector<int> &coeffsSelection) const
    {
        double res = 0;
        QVector<double> coeffs1 = other.getCoeffs();
        if (coeffsSelection.size() == coeffs.size()) {
            for (int i = 0; i < coeffs.size(); i++) {
                if (coeffsSelection.at(i))
                    res += (coeffs.at(i)-coeffs1.at(i))*(coeffs.at(i)-coeffs1.at(i));
            }
        } else {
            for (int i = 0; i < coeffs.size(); i++) {
                res += (coeffs.at(i)-coeffs1.at(i))*(coeffs.at(i)-coeffs1.at(i));
            }
        }
        return sqrt(res);
    }

private:
    void init();

    int tsNum;
    int windowNum;
    QVector<double> coeffs;
    double coeffsVectorLength;
};

#endif // DISTANCEELEMENT_H
