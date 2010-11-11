#include "common_math_tools.h"

#include <boost/numeric/ublas/lu.hpp>
#include <boost/math/tools/stats.hpp>
#include <boost/math/special_functions/factorials.hpp>

#include "pcg.hpp"

CommonMathTools::CommonMathTools()
{
//    currDataFrom = 0;
//    currDataTo = 0;
}

//int CommonMathTools::loadDataFromFiles(const QStringList & fileNames,
//                                       const int dataNorm_)
//{
//    dataNorm = dataNorm_;

//    tsCount = fileNames.count();

//    QVector<QFile *> files;
//    files.resize(tsCount);
//    for (int i = 0; i < tsCount; i++) {
//        QFile *file = new QFile(fileNames.at(i));
//        if (!file->open(QIODevice::ReadOnly)) {
//            emit infoMsg(QString("cannot open file: %1").arg(fileNames.at(i)));
//            return -1;
//        }
//        files[i] = file;
//    }

//    tsStdDev.resize(tsCount);
//    tsDisp.resize(tsCount);
//    tsMean.resize(tsCount);

//    tsValues.resize(tsCount);
//    tsValuesNorm.resize(tsCount);

//    for (int i = 0; i < tsCount; i++) {
//        int line = 0;
//        QString str;
//        bool ok;
//        double currNum = 0;

//        QVector<double> tmpVec;
//        while (!files.at(i)->atEnd()) {
//            str = files.at(i)->readLine();
//            str = str.split(' ').last();
//            currNum = str.toDouble(&ok);
//            if (!ok) {
//                emit infoMsg(QString("garbage at line %1 in file: %2")
//                             .arg(line).arg(fileNames.at(i)));
//                return -1;
//            }
//            tmpVec.append(currNum);
//            line++;
//        }
//        tsValues[i].resize(tmpVec.size());
//        for (unsigned int j = 0; j < tsValues[i].size(); j++) {
//            tsValues[i][j] = tmpVec.at(j);
//        }
//    }

//    for (int i = 0; i < tsCount; i++) {
//        files[i]->close();
//    }

//    for (int i = 0; i < tsCount; i++) {
//        delete files[i];
//    }

//    // проверяем, одинаковой ли длины файлы
//    for (int i = 1; i < tsCount; i++) {
//        if (tsValues[i].size() != tsValues[i-1].size()) {
//            emit infoMsg("timeseries has different lengths");
//            return -1;
//        }
//    }

//    calcStats();

//    // отнормируем ряды
//    normalizeTS();

//    return 0;
//}

//int CommonMathTools::loadDataFromFiles(const QStringList & fileNames,
//                                       const VECTOR_I &dataFrom,
//                                       const VECTOR_I &dataTo,
//                                       const int dataNorm_)
//{
//    dataNorm = dataNorm_;

//    tsCount = fileNames.count();

//    QVector<QFile *> files;
//    files.resize(tsCount);
//    for (int i = 0; i < tsCount; i++) {
//        QFile *file = new QFile(fileNames.at(i));
//        if (!file->open(QIODevice::ReadOnly)) {
//            emit infoMsg(QString("cannot open file: %1").arg(fileNames.at(i)));
//            return -1;
//        }
//        files[i] = file;
//    }

//    tsStdDev.resize(tsCount);
//    tsDisp.resize(tsCount);
//    tsMean.resize(tsCount);

//    tsValues.resize(tsCount);
//    tsValuesNorm.resize(tsCount);

//    for (int i = 0; i < tsCount; i++) {
//        int line = 0;
//        QString str;
//        bool ok;
//        double currNum = 0;

//        tsValues[i].resize(dataTo[i] - dataFrom[i]);
//        int k = 0;
//        for (int j = 0; j < dataFrom[i]; j++) {
//            files.at(i)->readLine();
//            line++;
//        }
//        while (k < (dataTo[i] - dataFrom[i])) {
//            str = files.at(i)->readLine();
//            str = str.split(' ').last();
//            currNum = str.toDouble(&ok);
//            if (!ok) {
//                emit infoMsg(QString("garbage at line %1 in file: %2")
//                             .arg(line).arg(fileNames.at(i)));
//                return -1;
//            }
//            tsValues[i][k] = currNum;
//            line++;
//            k++;
//        }
//    }

//    for (int i = 0; i < tsCount; i++) {
//        files[i]->close();
//    }

//    for (int i = 0; i < tsCount; i++) {
//        delete files[i];
//    }

//    // проверяем, одинаковой ли длины файлы
//    for (int i = 1; i < tsCount; i++) {
//        if (tsValues[i].size() != tsValues[i-1].size()) {
//            emit infoMsg("timeseries has different lengths");
//            return -1;
//        }
//    }

//    calcStats();

//    // отнормируем ряды
//    normalizeTS();

//    return 0;
//}

//void CommonMathTools::calcStats()
//{
//    for (int i = 0; i < tsCount; i++) {
//        boost::math::tools::stats<double> ts_stats;
//        int tsLen = tsValues[0].size();
//        for (int j = 0; j < tsLen; j++) {
//            ts_stats.add(tsValues[i][j]);
//        }
//        tsMean[i] = ts_stats.mean();
//        // ts_stats.variance1(); ?
//        tsDisp[i] = ts_stats.variance();
//        tsStdDev[i] = sqrt(tsDisp[i]);
//    }
//}

//void CommonMathTools::normalizeTS()
//{
//    int tsLen = tsValues[0].size();
//    for (int i = 0; i < tsCount; i++) {
//        tsValuesNorm[i].resize(tsLen);
//    }
//    // нормируем к нулевому среднему
//    // нормируем к единичной дисперсии
//    for (int j = 0; j < tsLen; j++) {
//        for (int i = 0; i < tsCount; i++) {
//            tsValuesNorm[i][j] = tsValues[i][j] - tsMean[i];
//            tsValuesNorm[i][j] /= tsStdDev[i];
//        }
//    }
//}

//int CommonMathTools::calcResiduals(const VECTOR_M & ar_coeffs,
//                                   const int dimension,
//                                   const VECTOR_I & ts_nums,
//                                   const int shift,
//                                   VECTOR_VD & residuals)
//{
//    int tsLen = getTSlen();

//    // готовим вектор векторов остатков моделей
//    VECTOR_VD Res;
//    Res.resize(ts_nums.size());
//    for (unsigned int i = 0; i < ts_nums.size(); i++) {
//        Res[i].resize(tsLen - dimension - shift);
//    }

//    for (int i = dimension + shift; i < (tsLen-abs(shift)); i++) {
//        for (unsigned int s1 = 0; s1 < ts_nums.size(); s1++) {
//            double x_mine = 0;

//            // вычислим значение x_mine временного ряда ts_nums.at(s1) в точке i,
//            // определяемое коэффициентами АР-моделей ar_coeffs всех систем ts_nums
//            for (unsigned int s2 = 0; s2 < ts_nums.size(); s2++) {
//                for (int p = 0; p < dimension; p++) {
//                    x_mine += getTSvalueNorm(ts_nums[s2],
//                                             i - (p+1 + shift))
//                    * ar_coeffs[p](ts_nums[s1], ts_nums[s2]);
//                }
//            }

//            double x_real = getTSvalueNorm(ts_nums[s1], i);
//            Res[s1][i - (dimension + shift)] = x_real - x_mine;
//        }
//    }

//    // возвращаем результат
//    residuals = Res;

//    return 0;
//}

//int CommonMathTools::calcResiduals_ts1(const VECTOR_D & ar_coeffs,
//                                       const int dimension,
//                                       const int order,
//                                       const int base_ts,
//                                       VECTOR_D & residuals)
//{
//    int tsLen = getTSlen();

//    int coeffsCount = boost::math::factorial<double>(dimension*1 + order)
//                      /(boost::math::factorial<double>(dimension*1)*boost::math::factorial<double>(order));

//    prepearePowers(dimension, order);

//    // готовим вектор остатков моделей
//    VECTOR_D Res;
//    Res.resize(tsLen - dimension);

//    for (int i = dimension; i < tsLen; i++) {
//        double x_mine = 0;

//        // вычислим значение x_mine временного ряда base_ts в точке i,
//        // определяемое коэффициентами АР-моделей ar_coeffs
//        for (int c = 0; c < coeffsCount; c++) {
//            x_mine += getBasisFuncValue_single_ts(c, dimension, order, base_ts, i)
//                      * ar_coeffs[c];
//        }

//        double x_real = getTSvalueNorm(base_ts, i);
//        Res[i - dimension] = x_real - x_mine;
//    }

//    // возвращаем результат
//    residuals = Res;

//    return 0;
//}

//void CommonMathTools::setDataWindow(const int dataFrom, const int dataTo)
//{
//    currDataFrom = dataFrom;
//    currDataTo = dataTo;
//    calcStats();
//}

//void CommonMathTools::lls_solve(int base_ts_index,
//                                const VECTOR_I &tsIndexes,
//                                int dimension,
//                                VECTOR_D *ar_coeffs)
//{
//    int tsLen = getTSlen();
//    int coeffs_cnt = dimension*(tsIndexes.size() + 1);

//    MATRIX A(coeffs_cnt, coeffs_cnt);
//    for (unsigned int i = 0; i < A.size1(); i++)
//        for (unsigned int j = 0; j < A.size2(); j++)
//            A(i, j) = 0;

//    VECTOR_D B(coeffs_cnt);
//    for (unsigned int i = 0; i < B.size(); i++)
//        B(i) = 0;

//    VECTOR_D C(coeffs_cnt);
//    for (unsigned int i = 0; i < C.size(); i++)
//        C(i) = 0;

//    for (int Ni = dimension; Ni < tsLen; Ni++) {
//        for (int Pi = 0; Pi < coeffs_cnt; Pi++) {
//            for (int Pj = 0; Pj < coeffs_cnt; Pj++) {
//                A(Pi, Pj) += getBasisFuncValue(Pi, dimension, base_ts_index, tsIndexes, Ni)
//                             *getBasisFuncValue(Pj, dimension, base_ts_index, tsIndexes, Ni);
//            }
//            B(Pi) += getBasisFuncValue(Pi, dimension, base_ts_index, tsIndexes, Ni)
//                     *getTSvalueNorm(base_ts_index, Ni);
//        }
//    }

//    gaussSolve(A, B, C);

//    for (int i = 0; i < coeffs_cnt; i++)
//        (*ar_coeffs)[i] = C[i];
//}

//double CommonMathTools::getBasisFuncValue(int Pi, int dimension,
//                                          int base_ts_index, const VECTOR_I &tsIndexes,
//                                          int Ni)
//{
//    int ts_idx = (Pi < dimension) ? base_ts_index :
//                 tsIndexes[(Pi - dimension)/dimension];
//    int x_idx = Pi%dimension + 1;
//    return getTSvalueNorm(ts_idx, Ni-x_idx);
//}

//void CommonMathTools::lls_solve_ts1(int base_ts_index,
//                                    int dimension,
//                                    VECTOR_D *ar_coeffs,
//                                    int order)
//{
//    int tsLen = getTSlen();
//    int coeffs_cnt = boost::math::factorial<double>(dimension*1 + order)
//                     /(boost::math::factorial<double>(dimension*1)*boost::math::factorial<double>(order));

//    prepearePowers(dimension, order);

//    MATRIX A(coeffs_cnt, coeffs_cnt);
//    for (unsigned int i = 0; i < A.size1(); i++)
//        for (unsigned int j = 0; j < A.size2(); j++)
//            A(i, j) = 0;

//    VECTOR_D B(coeffs_cnt);
//    for (unsigned int i = 0; i < B.size(); i++)
//        B(i) = 0;

//    VECTOR_D C(coeffs_cnt);
//    for (unsigned int i = 0; i < C.size(); i++)
//        C(i) = 0;

//    for (int Ni = dimension; Ni < tsLen; Ni++) {
//        for (int Pi = 0; Pi < coeffs_cnt; Pi++) {
//            for (int Pj = 0; Pj < coeffs_cnt; Pj++) {
//                A(Pi, Pj) += getBasisFuncValue_single_ts(Pi, dimension, order, base_ts_index, Ni)
//                             *getBasisFuncValue_single_ts(Pj, dimension, order, base_ts_index, Ni);
//            }
//            B(Pi) += getBasisFuncValue_single_ts(Pi, dimension, order, base_ts_index, Ni)
//                     *getTSvalueNorm(base_ts_index, Ni);
//        }
//    }

//    gaussSolve(A, B, C);

//    for (int i = 0; i < coeffs_cnt; i++)
//        (*ar_coeffs)[i] = C[i];
//}

//функция инверсии битов относительно медианы. p - порядок, т.е. общее количество бит
////например, если p=4 (числа i отображены в двоичном представлении)
////p(0001,4) = 1000
////p(0011,4) = 1100
////p(0111,4) = 1110
////p(1010,4) = 0101
////данный алгоритм требуется для подготовки исходного массива быстрого Фурье преобразования
////порядка p (например, для 1024-точечной схемы p=10)
//int CommonMathTools::reverse(int i, int p)
//{
//    int shift = p-1;
//    unsigned int low_mask = 1;
//    unsigned int high_mask = 1 << shift;
//    unsigned int res;

//    for (res=0; shift>=0; low_mask <<= 1, high_mask >>= 1, shift -= 2)
//        res |= ((i & low_mask) << shift) | ((i & high_mask) >> shift);
//    return res;
//}

//быстрое преобразование Фурье
//p - порядок задачи, т.е для 1024-точечного пребразования Фурье p=10 (2^p = N)
//in - входной массив отсчетов сигнала размерности 2^p
//-----------------------------
//Возможная оптимизация: алгоритм написан с учетом правильного масштабирования
//спектральных составляющих. Если это не нужно, то можно убрать умножение
//отсчетов сигнала на 2 при подготовке исходных данных, и умножение на 0.5
//в самом внутреннем цикле преобразования.
//VECTOR_C CommonMathTools::fft(int p, const VECTOR_D &in)
//{
//    int n_max = 1 << p;
//    std::complex<double> w,foo;
//    VECTOR_C out;
//    double pi2 = 2 * 3.1415926535897932384626433832795;
//    double dummy;
//    //Nd2 - это N делить на 2
//    //kpNd2 - это k плюс Nd2
//    int n,Nd2,k,kpNd2,m;

//    //обнулим массив спектральных отсчетов
//    out.resize(n_max);
//    for (int i = 0 ; i < n_max; i++) {
//        out[i].real(0);
//        out[i].imag(0);
//    }

//    //готовим массив исходных данных (p-мерная сортировка по четности)
//    for (n=0;n<n_max;n++) {
//        k = reverse(n,p);
//        out[k].real(in[n]*2);
//    }

//    //собственно само преобразование
//    for (n=2, Nd2=1; n<=n_max; Nd2=n, n+=n)
//    {
//        for (k=0;k<Nd2;k++) {
//            //w := cexp(-I*pi2*k/n);
//            dummy = pi2*k/n;
//            w.real(cos(dummy));
//            w.imag(-sin(dummy));

//            for(m=k; m < n_max; m +=n) {
//                kpNd2 = m + Nd2;
//                foo.real(w.real()*out[kpNd2].real()-w.imag()*out[kpNd2].imag());
//                foo.imag(w.real()*out[kpNd2].imag()+w.imag()*out[kpNd2].real());

//                out[kpNd2].real() = (out[m].real() - foo.real())*0.5;
//                out[kpNd2].imag() = (out[m].imag() - foo.imag())*0.5;

//                out[m].real() = (out[m].real() + foo.real())*0.5;
//                out[m].imag() = (out[m].imag() + foo.imag())*0.5;
//            }
//        }
//    }
//    return out;
//}

int CommonMathTools::gaussSolve(const MATRIX& A,
                                const VECTOR_D& B,
                                VECTOR_D& X)
{
    //    int N = A.size1();
    //
    //    //    double det = 0;
    //    //    calcDeterminant(A, det);
    //    //    if (fabs(det) < ZERO) {
    //    //        return -1;
    //    //    }
    //
    //    MATRIX Ac = A;
    //    boost::numeric::ublas::permutation_matrix<double> P(N);
    //    VECTOR x(N);
    //
    //    boost::numeric::ublas::lu_factorize(Ac, P);
    //    x = B;
    //    boost::numeric::ublas::lu_substitute(Ac, P, x);
    //    X = x;
    //    return 0;
    typedef CholeskyPreconditioner<MATRIX> PRECOND;
    PRECOND precond(A);
    pcg_solve<MATRIX, VECTOR_D, PRECOND>(A, X, B, precond, 5000, 1e-10, 1e-20);
    return 0;
}

int CommonMathTools::calcInverseMatrix(const MATRIX& A,
                                       MATRIX& B)
{
    // create a working copy of the input
    MATRIX mLu(A);

    // perform LU-factorization
    lu_factorize(mLu);

    // create identity matrix of "inverse"
    B.assign(boost::numeric::ublas::identity_matrix<double> (A.size1()));

    // backsubstitute to get the inverse
    boost::numeric::ublas::lu_substitute<MATRIX const, MATRIX >(mLu, B);
    return 0;
}

int CommonMathTools::calcDeterminant(const MATRIX& A,
                                     double & Det)
{
    // create a working copy of the input
    MATRIX aLu(A);
    boost::numeric::ublas::permutation_matrix<std::size_t> pivots(A.size1());

    boost::numeric::ublas::lu_factorize(aLu, pivots);

    double det = 1.0;

    for (std::size_t i = 0; i < pivots.size(); ++i) {
        if (pivots(i) != i)
            det *= -1.0;
        det *= aLu(i, i);
    }
    Det = det;
    return 0;
}

void CommonMathTools::prepearePowers(int dimension, int order,
                                     QVector< QVector<int> > *powers)
{
    powers->resize(0);
    int m = dimension;
    for (int n = 0; n <= order; n++) {
        QVector<int> powersTmp;
        powersTmp.resize(dimension);
        powersTmp.fill(0);
        while (true) {
            int sum = 0;
            for (int i = 0; i < m; i++) sum += powersTmp[i];
            if (sum == n) powers->append(powersTmp);

            powersTmp[0]++;
            for (int i = 0; i < m-1; i++) {
                if (powersTmp[i] >= (n+1)) {
                    powersTmp[i] = 0;
                    powersTmp[i+1]++;
                }
                else break;
            }

            if (powersTmp[m-1] >= (n+1)) break;
        }
    }
}

double CommonMathTools::getBasisFuncValue_single_ts(int Pi, int dimension, int Ni,
                                                    const QVector< QVector<int> > &powers,
                                                    const VECTOR_D &data)
{
    double res = 1;
    for (int i = 0; i < dimension; i++) {
        if (powers.at(Pi).at(i) == 0) continue;
        if (powers.at(Pi).at(i) == 1) {
            res *= data[Ni-(i+1)];
            continue;
        }
        if (powers.at(Pi).at(i) == 2) {
            res *= data[Ni-(i+1)]*data[Ni-(i+1)];
            continue;
        }
        res *= pow(data[Ni-(i+1)], powers.at(Pi).at(i));
    }
    return res;
}

void CommonMathTools::lls_solve_single_ts(int dimension, int order,
                                          const VECTOR_D &data,
                                          VECTOR_D *ar_coeffs)
{
    int tsLen = data.size();
    int coeffs_cnt = boost::math::factorial<double>(dimension*1 + order)
                     /(boost::math::factorial<double>(dimension*1)*boost::math::factorial<double>(order));

    QVector< QVector<int> > powers;
    prepearePowers(dimension, order, &powers);

    MATRIX A(coeffs_cnt, coeffs_cnt);
    for (unsigned int i = 0; i < A.size1(); i++)
        for (unsigned int j = 0; j < A.size2(); j++)
            A(i, j) = 0;

    VECTOR_D B(coeffs_cnt);
    for (unsigned int i = 0; i < B.size(); i++)
        B(i) = 0;

    VECTOR_D C(coeffs_cnt);
    for (unsigned int i = 0; i < C.size(); i++)
        C(i) = 0;

    for (int Ni = dimension; Ni < tsLen; Ni++) {
        for (int Pi = 0; Pi < coeffs_cnt; Pi++) {
            for (int Pj = 0; Pj < coeffs_cnt; Pj++) {
                A(Pi, Pj) += getBasisFuncValue_single_ts(Pi, dimension, Ni, powers, data)
                             *getBasisFuncValue_single_ts(Pj, dimension, Ni, powers, data);
            }
            B(Pi) += getBasisFuncValue_single_ts(Pi, dimension, Ni, powers, data)
                     *data[Ni];
        }
    }

    gaussSolve(A, B, C);

    for (int i = 0; i < coeffs_cnt; i++)
        (*ar_coeffs)[i] = C[i];
}
