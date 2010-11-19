#include "splitsclusterisationreport.h"
#include "ui_splitsclusterisationreport.h"

#include "distanceelement.h"
#include "datakeeper.h"

SplitsClusterisationReport::SplitsClusterisationReport(SplitsClusterisation *splitsClusterisation, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplitsClusterisationReport),
    splitsClusterisation(splitsClusterisation)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
}

SplitsClusterisationReport::~SplitsClusterisationReport()
{
    delete ui;
}

void SplitsClusterisationReport::on_updateRepButton_clicked()
{
    int tsCount = splitsClusterisation->getDataKeepers()->size();
    QMap<int, QVector<DistanceElement> > *clusters = splitsClusterisation->getClusters();
    QMap<int, QString> clustersColors;

    QStringList colorNames = QColor::colorNames();
    int k = 0;
    foreach (int cluster, clusters->keys()) {
        clustersColors[cluster] = colorNames.at(k+10);
        k++;
    }
    
    ui->clustersRepTableWidget->setRowCount(tsCount);
    
    for (int i = 0; i < tsCount; i++) {
        QWidget *w = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        w->setLayout(layout);

        foreach (int cluster, clusters->keys()) {
            int windowsCount = splitsClusterisation->getDataKeepers()->at(i)->getWindowsCount();
            int partsCount = 0;
            QVector<DistanceElement> distanceElements = clusters->value(cluster);
            
            for (int k = 0; k < distanceElements.size(); k++) {
                if (distanceElements.at(k).getTsNum() == i) partsCount++;
            }
            
            if (partsCount > 0) {
                int percentageRatio = 100*partsCount/windowsCount;

                QWidget *tmpW = new QWidget;
                tmpW->setStyleSheet(QString("background: %1").arg(clustersColors[cluster]));
             
                layout->addWidget(tmpW);
                layout->setStretchFactor(tmpW, percentageRatio);
            }
        }
      
        QString name = QFileInfo(splitsClusterisation->getDataKeepers()->at(i)->getDataFileName()).baseName();
        QTableWidgetItem *item1 = new QTableWidgetItem(name);
        ui->clustersRepTableWidget->setItem(i, 0, item1);
        
        ui->clustersRepTableWidget->setCellWidget(i, 1, w);
    }
    
    ui->clustersRepTableWidget->resizeColumnsToContents();
    
#if 0
    QMap<int, QVector<NSDistanceElement> > *clusters = splitsClusterisation->getClusters();
    
    int rowTs = 0;
    foreach (int cluster, clusters->keys()) {
        int tsCount = 0;
        for (int j = 0; j < clusters->value(cluster).size(); j++) {
            if (i.value().at(j).getTsNum() == 1) ts1Count++;
            if (i.value().at(j).getTsNum() == 2) ts2Count++;
        }
        if (ts1Count > 0) {
            modelTs1->setRowCount(rowTs1+1);
            QModelIndex index;
            index = modelTs1->index(rowTs1, 0, QModelIndex());
            modelTs1->setData(index, QVariant(i.key()), Qt::DisplayRole);
            index = modelTs1->index(rowTs1, 1, QModelIndex());
            modelTs1->setData(index, QVariant(ts1Count), Qt::DisplayRole);
            index = modelTs1->index(rowTs1, 2, QModelIndex());
            modelTs1->setData(index, QVariant(100*ts1Count/(double)elementsCount), Qt::DisplayRole);
            index = modelTs1->index(rowTs1, 3, QModelIndex());
//            QPushButton *button = new QPushButton(trUtf8("сохранить..."));
//            connect(button, SIGNAL(clicked()), this, SLOT(saveCurrentClusterTs1()));
//            ui->ts1RepTableView->setIndexWidget(index, button);
            modelTs1->setData(index, QVariant(trUtf8("сохранить...")), Qt::DisplayRole);
            index = modelTs1->index(rowTs1, 4, QModelIndex());
            modelTs1->setData(index, QVariant(trUtf8("спектр...")), Qt::DisplayRole);
            rowTs1++;
        }
        if (ts2Count > 0) {
            modelTs2->setRowCount(rowTs2+1);
            QModelIndex index;
            index = modelTs2->index(rowTs2, 0, QModelIndex());
            modelTs2->setData(index, QVariant(i.key()), Qt::DisplayRole);
            index = modelTs2->index(rowTs2, 1, QModelIndex());
            modelTs2->setData(index, QVariant(ts2Count), Qt::DisplayRole);
            index = modelTs2->index(rowTs2, 2, QModelIndex());
            modelTs2->setData(index, QVariant(100*ts2Count/(double)elementsCount), Qt::DisplayRole);
            index = modelTs2->index(rowTs2, 3, QModelIndex());
//            QPushButton *button = new QPushButton(trUtf8("сохранить..."));
//            connect(button, SIGNAL(clicked()), this, SLOT(saveCurrentClusterTs2()));
//            ui->ts2RepTableView->setIndexWidget(index, button);
            modelTs2->setData(index, QVariant(trUtf8("сохранить...")), Qt::DisplayRole);
            index = modelTs2->index(rowTs2, 4, QModelIndex());
            modelTs2->setData(index, QVariant(trUtf8("спектр...")), Qt::DisplayRole);
            rowTs2++;
        }
        if ((ts1Count > 0) && (ts2Count > 0)) {
            modelTs12->setRowCount(rowTs12+1);
            QModelIndex index;
            index = modelTs12->index(rowTs12, 0, QModelIndex());
            modelTs12->setData(index, QVariant(i.key()), Qt::DisplayRole);
            index = modelTs12->index(rowTs12, 1, QModelIndex());
            modelTs12->setData(index, QVariant(QString("%1/%2").arg(ts1Count).arg(ts2Count)), Qt::DisplayRole);
            index = modelTs12->index(rowTs12, 2, QModelIndex());
            modelTs12->setData(index, QVariant(100*(ts1Count+ts2Count)/(double)(2*elementsCount)), Qt::DisplayRole);
            index = modelTs12->index(rowTs12, 3, QModelIndex());
            modelTs12->setData(index, QVariant((ts2Count-ts1Count) > 0 ?
                                               QString("+%1 (+%2\%)").arg(ts2Count-ts1Count).arg(100*(ts2Count-ts1Count)/ts1Count) :
                                               QString("%1 (%2\%)").arg(ts2Count-ts1Count).arg(100*(ts2Count-ts1Count)/ts1Count)), Qt::DisplayRole);
            rowTs12++;
        }
    }
    ui->clustersRepTableView->setSortingEnabled(true);
    ui->clustersTableView->sortByColumn(2, Qt::DescendingOrder);
    ui->clustersRepTableView->resizeColumnsToContents();
#endif
}

void SplitsClusterisationReport::on_saveRepButton_clicked()
{
#if 0
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Сохранить отчет"),
                                                    "", trUtf8("Текстовые файлы (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);

    out << trUtf8("Отчет по сравнению двух временных рядов с помощью кластеризации.\n");
    out << "\n";
    out << trUtf8("сравниваемые временные ряды:\n");
    out << trUtf8("ряд1: ") << ts1FileName << "\n";
    out << trUtf8("ряд2: ") << ts2FileName << "\n";
    out << trUtf8("параметры:\n");
    out << trUtf8("данные  ряд1 от: ") << ts1From << trUtf8(" до: ") << ts1To << "\n";
    out << trUtf8("        ряд2 от: ") << ts2From << trUtf8(" до: ") << ts2To << "\n";
    out << trUtf8("        окно: ") << window << "\n";
    out << trUtf8("        смещение: ") << step << "\n";
    out << trUtf8("        ширина кластера: ") << clusterLength << "\n";
    out << trUtf8("метод размерность: ") << dimension << "\n";
    out << trUtf8("      порядок: ") << order << "\n";
    out << trUtf8("кластеризация, разделение на кластеры: ") << clustersCount << "\n";
    out << "\n";
    out << trUtf8("типы поведения (кластеры) в первом временном ряду:\n");
    out << qSetFieldWidth(6) << trUtf8("№; ") << trUtf8("кол-во; ") << trUtf8("% от общ. кол-ва") << "\n";
    for (int i = 0; i < modelTs1->rowCount(); i++) {
        out << qSetFieldWidth(6) << modelTs1->item(i, 0)->data(Qt::DisplayRole).toString() << " "
                << modelTs1->item(i, 1)->data(Qt::DisplayRole).toString() << " "
                << modelTs1->item(i, 2)->data(Qt::DisplayRole).toInt() << "\n";
    }
    out << trUtf8("типы поведения (кластеры) во втором временном ряду:\n");
    out << trUtf8("№; ") << trUtf8("кол-во; ") << trUtf8("% от общ. кол-ва ") << "\n";
    for (int i = 0; i < modelTs2->rowCount(); i++) {
        out << qSetFieldWidth(6) << modelTs2->item(i, 0)->data(Qt::DisplayRole).toString() << " "
                << modelTs2->item(i, 1)->data(Qt::DisplayRole).toString() << " "
                << modelTs2->item(i, 2)->data(Qt::DisplayRole).toInt() << "\n";
    }
    out << trUtf8("общие типы поведения (кластеры) сравнение:\n");
    out << trUtf8("№; ") << trUtf8("ряд1/ряд2; ") << trUtf8("% от общ. кол-ва; ") << trUtf8("изменение ") << "\n";
    for (int i = 0; i < modelTs12->rowCount(); i++) {
        out << qSetFieldWidth(6) << modelTs12->item(i, 0)->data(Qt::DisplayRole).toString() << " "
                << modelTs12->item(i, 1)->data(Qt::DisplayRole).toString() << " "
                << modelTs12->item(i, 2)->data(Qt::DisplayRole).toInt() << " "
                << modelTs12->item(i, 3)->data(Qt::DisplayRole).toString() << "\n";
    }
#endif
}
