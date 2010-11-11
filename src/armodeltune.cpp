#include "armodeltune.h"
#include "ui_armodeltune.h"

#include "zchart.h"
#include "distanceelement.h"
#include "common_math_tools.h"

ArModelTune::ArModelTune(ArDataAnalysis *arDataAnalysis, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArModelTune),
    arDataAnalysis(arDataAnalysis)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window);

    coeffsChart = new ZChart();
    coeffsChart->setBorderType(0);
    coeffsChart->setGraphPanelBorderType(0);
    coeffsChart->setMargin(2);
    coeffsChart->setSpacing(2);
    coeffsChart->setFirstText("");
    coeffsChart->setSecondText(" ");
    coeffsChart->setTitle(trUtf8("коэффициенты временных рядов"));
    coeffsChart->setXRange(0, 10);
    coeffsChart->setYRange(-10, 10);
    coeffsChart->setAxesName(trUtf8(""), "");
    coeffsChart->showLegend(true);
    coeffsChart->setAutoscale(true);

    swartzChart = new ZChart();
    swartzChart->setBorderType(0);
    swartzChart->setGraphPanelBorderType(0);
    swartzChart->setMargin(2);
    swartzChart->setSpacing(2);
    swartzChart->setFirstText("");
    swartzChart->setSecondText(" ");
    swartzChart->setTitle(trUtf8("оптимальные размерности для участков"));
    swartzChart->setXRange(0, 10);
    swartzChart->setYRange(-10, 10);
    swartzChart->setAxesName(trUtf8(""), "");
    swartzChart->showLegend(false);
    swartzChart->setAutoscale(true);

    swartzSeries = new ZSimpleSeries(trUtf8(""));
    swartzSeries->setColor(Qt::red);
    swartzChart->addSeries(swartzSeries);

    ui->gridLayout_2->addWidget(coeffsChart, 0, 0, 1, 1);
    ui->gridLayout->addWidget(swartzChart, 3, 0, 1, 2);

    progress = new QProgressDialog(trUtf8("Идет расчет..."), trUtf8("Отмена"), 0, 1, this);
    progress->setWindowModality(Qt::ApplicationModal);
    progress->setMinimumDuration(0);
}

ArModelTune::~ArModelTune()
{
    delete ui;
}

void ArModelTune::on_updateGraphsButton_clicked()
{
    updateGraphs(ui->normalizeCoeffsChk->isChecked());
}

void ArModelTune::updateGraphs(bool normalize)
{
    QVector<DistanceElement> *distanceElements = arDataAnalysis->getDistanceElements();

    int coeffsCount = distanceElements->at(0).getCoeffs().size();
    int partsCount = distanceElements->size();

    QVector<double> maxes;
    maxes.resize(coeffsCount);
    maxes.fill(0);
    if (normalize) {
        for (int i = 0; i < coeffsCount; i++) {
            for (int j = 0; j < partsCount; j++) {
                if (maxes.at(i) < fabs(distanceElements->at(j).getCoeffs().at(i)))
                    maxes[i] = fabs(distanceElements->at(j).getCoeffs().at(i));
            }
        }
    } else {
        maxes.fill(1);
    }

    coeffsChart->removeSeries();
    coeffsSerieses.clear();

    for (int i = 0; i < coeffsCheckBoxes.size(); i++)
        coeffsCheckBoxes.at(i)->deleteLater();
    coeffsCheckBoxes.clear();

    QStringList colorNames = QColor::colorNames();
    for (int i = 0; i < coeffsCount; i++) {
        ZSimpleSeries *ser = new ZSimpleSeries(QString("%1").arg(i));
        ser->setColor(QColor(colorNames.at(i+10)));
        coeffsSerieses.append(ser);
        ser->clear();
        coeffsChart->addSeries(ser);

        for (int j = 0, k = 0; j < partsCount; j++, k++) {
            ser->add(k,
                     distanceElements->at(j).getCoeffs().at(i)/maxes.at(i));
        }

        QCheckBox *cb = new QCheckBox(QString("c%1").arg(i));
        coeffsCheckBoxes.append(cb);
        cb->setChecked(true);
        connect(cb, SIGNAL(clicked()), this, SLOT(updateCoeffsSelection()));
        ui->coeffsChooseLayout->addWidget(cb, i%3, i/3);
    }

    coeffsChart->autoscale();

    showModel();
}

void ArModelTune::showModel()
{
    CommonMathTools *cmtObj = new CommonMathTools();
    QString model = "x_n = c0 + ";

    QVector< QVector<int> > powers;
    cmtObj->prepearePowers(arDataAnalysis->getDimension(),
                           arDataAnalysis->getOrder(),
                           &powers);

    int coeffsCount = powers.size();
    for (int c = 1; c < coeffsCount; c++) {
        QString monom = QString("c%1 ").arg(c);
        for (int d = 0; d < powers.at(c).size(); d++) {
            if (powers.at(c).at(d) == 1) {
                monom += QString("x_n-%1 ").arg(d+1);
            }
            if (powers.at(c).at(d) > 1) {
                monom += QString("x^%1_n-%2 ").arg(powers.at(c).at(d)).arg(d+1);
            }
        }
        model += monom;
        model += " + ";
    }
    model.chop(3);
    ui->arModelEdit->setPlainText(model);

    delete cmtObj;
}

void ArModelTune::on_normalizeCoeffsChk_clicked()
{
    on_updateGraphsButton_clicked();
}

void ArModelTune::updateCoeffsSelection()
{
    coeffsSelection.clear();
    for (int i = 0; i < coeffsCheckBoxes.size(); i++) {
        coeffsSelection.append(coeffsCheckBoxes.at(i)->isChecked());
    }
}

int ArModelTune::getEstimatedTime()
{
    return 0;
}

void ArModelTune::on_calcOptimumDimension_clicked()
{
#if 0
    int order = ui->optimumDimOrderEdit->text().toInt();
    int maxDim = ui->optimumDimMaxEdit->text().toInt();
    int partsCount = distanceElements1.size();

    dimensions1Graph->clear();
    dimensions2Graph->clear();

    progress->setMaximum(2*partsCount+1);
    progress->setValue(0);

    for (int tsNum = 0; tsNum < 2; tsNum++) {
        for (int i = 0; i < partsCount; i++) {
            int dim = nsshell->calcOptDim(tsNum, i, maxDim, order);
            if (tsNum == 0) {
                dimensions1Graph->add(ts1From +step*(i+1),
                                      dim);
            }
            if (tsNum == 1) {
                dimensions2Graph->add(ts2From +step*(i+1),
                                      dim);
            }

            progress->setValue(progress->value()+1);
            if (progress->wasCanceled()) {
                swartzChart->autoscale();
                progress->setValue(progress->maximum());
                return;
            }
        }
    }

    swartzChart->autoscale();
    progress->setValue(progress->maximum());
#endif
}
