#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class DataKeeper;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QList<DataKeeper *> dataKeepers;

private slots:
    void on_addDataButton_clicked();
    void on_timeSeriesesTabs_tabCloseRequested(int index);
};

#endif // MAINWINDOW_H
