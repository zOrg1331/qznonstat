#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class QProgressDialog;

class DataKeeper;
class DistanceElement;
class ArDataAnalysis;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void refreshDataTabsText();
    void fillDataSettingsWidgets(DataKeeper *dataKeeper);
    void disableDataSettingsWidgets(bool disable);

    Ui::MainWindow *ui;
    QProgressDialog *progress;

    QList<DataKeeper *> dataKeepers;

    QVector<DistanceElement> distanceElements;

    ArDataAnalysis *arDataAnalysisRoutine;

private slots:
    void on_addDataButton_clicked();
    void on_timeSeriesesTabs_tabCloseRequested(int index);
    void on_loadDataButton_clicked();
    void on_dataFromEdit_editingFinished();
    void on_dataToEdit_editingFinished();
    void on_dataWindowEdit_editingFinished();
    void on_dataStepEdit_editingFinished();
    void on_timeSeriesesTabs_currentChanged(int index);
    void on_dataAllCheck_clicked(bool checked);
    void on_dataSecondsCheck_clicked(bool checked);
    void on_dataSamplingEdit_editingFinished();
    void on_doDataAnalysisButton_clicked();
};

#endif // MAINWINDOW_H
