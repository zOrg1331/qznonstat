#ifndef COMMONMATHTOOLS_H
#define COMMONMATHTOOLS_H

#include <QtCore>

#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/lu.hpp>

typedef boost::numeric::ublas::matrix<double> MATRIX;
typedef boost::numeric::ublas::matrix<std::complex<double> > MATRIXcmplx;
typedef boost::numeric::ublas::vector<double> VECTOR_D;
typedef boost::numeric::ublas::vector<int> VECTOR_I;
typedef boost::numeric::ublas::vector<std::complex<double> > VECTOR_C;
typedef boost::numeric::ublas::vector<VECTOR_I > VECTOR_VI;
typedef boost::numeric::ublas::vector<MATRIX > VECTOR_M;
typedef boost::numeric::ublas::vector<VECTOR_M > VECTOR_VM;
typedef boost::numeric::ublas::vector<VECTOR_D > VECTOR_VD;
typedef boost::numeric::ublas::vector<VECTOR_VD > VECTOR_VVD;
typedef boost::numeric::ublas::vector<VECTOR_VVD > VECTOR_VVVD;

class CommonMathTools : public QObject
{
    Q_OBJECT

public:
    CommonMathTools();

//    int loadDataFromFiles(const QStringList & fileNames,
//                          const VECTOR_I & dataFrom,
//                          const VECTOR_I & dataTo,
//                          const int dataNorm = 0);

//    int loadDataFromFiles(const QStringList & fileNames,
//                          const int dataNorm = 0);

//    int getTScount() { return tsCount; }
//    int getTSlen() {
//        if ((currDataFrom == 0) && (currDataTo == 0)) return tsValues[0].size();
//        else return (currDataTo-currDataFrom);
//    }
//    int getTSlenAbs() { return tsValues[0].size(); }

//    double getMean(const int tsNum) { return tsMean[tsNum]; }
//    double getDisp(const int tsNum) { return tsDisp[tsNum]; }
//    double getStdDeviation(const int tsNum) { return tsStdDev[tsNum]; }

//    double getTSvalue(const int TSNum,
//                      const int num)
//    {
//        if (dataNorm) return getTSvalueNorm(TSNum, num);
//        int tsLen = tsValues[0].size();
//        if ((currDataFrom == 0) && (currDataTo == 0)) {
//            if ((num >= 0) && (num < tsLen)) return tsValues[TSNum][num];
//            else return 0;
//        } else {
//            if (((num+currDataFrom) >= 0) && (num < currDataTo))
//                return tsValues[TSNum][num+currDataFrom];
//            else return 0;
//        }
//    }

//    double getTSvalueNorm(const int TSNum,
//                          const int num)
//    {
//        if (!dataNorm) return getTSvalue(TSNum, num);
//        int tsLen = tsValues[0].size();
//        if ((currDataFrom == 0) && (currDataTo == 0)) {
//            if ((num >= 0) && (num < tsLen)) return tsValuesNorm[TSNum][num];
//            else return 0;
//        } else {
//            if (((num+currDataFrom) >= 0) && (num < currDataTo))
//                return tsValuesNorm[TSNum][num+currDataFrom];
//            else return 0;
//        }
//    }

//    int calcResiduals(const VECTOR_M & ar_coeffs,
//                      const int dimension,
//                      const VECTOR_I & ts_nums,
//                      const int shift,
//                      VECTOR_VD & residuals);

//    int calcResiduals_ts1(const VECTOR_D & ar_coeffs,
//                          const int dimension,
//                          const int order,
//                          const int base_ts,
//                          VECTOR_D & residuals);

//    void setDataWindow(const int dataFrom, const int dataTo);

//    void lls_solve(const int base_ts_index,
//                   const VECTOR_I & tsIndexes,
//                   const int dimension,
//                   VECTOR_D *ar_coeffs);

//    double getBasisFuncValue(int Pi, int dimension,
//                             int base_ts_index, const VECTOR_I &tsIndexes,
//                             int Ni);

//    void lls_solve_ts1(const int base_ts_index,
//                       const int dimension,
//                       VECTOR_D *ar_coeffs,
//                       int order = 1);

    static void calcStats(const VECTOR_D &data,
                          double *mean, double *disp, double *stddev);

    static void calcStats(const QVector<double> &data,
                          double *mean, double *disp, double *stddev);

    int gaussSolve(const MATRIX& A,
                   const VECTOR_D& B,
                   VECTOR_D& X);

    int calcInverseMatrix(const MATRIX& A,
                          MATRIX& B);

    int calcDeterminant(const MATRIX& A,
                        double & Det);

    static void prepearePowers(int dimension, int order, QVector< QVector<int> > *powers);

    void lls_solve_single_ts(int dimension, int order,
                             const VECTOR_D &data,
                             VECTOR_D *ar_coeffs);

//    VECTOR_C fft(int p, const VECTOR_D &in);

private:
//    void normalizeTS();

    double getBasisFuncValue_single_ts(int Pi, int dimension, int Ni,
                                       const QVector< QVector<int> > &powers,
                                       const VECTOR_D &data);
//    int reverse(int i, int p);

//    VECTOR_VD tsValues;
//    VECTOR_VD tsValuesNorm;
//    VECTOR_D tsStdDev;
//    VECTOR_D tsDisp;
//    VECTOR_D tsMean;
//    int tsCount;

//    int currDataFrom;
//    int currDataTo;

//    int dataNorm;

signals:
    void infoMsg(QString);

};

#endif // COMMONMATHTOOLS_H
