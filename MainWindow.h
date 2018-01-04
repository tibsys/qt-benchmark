#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <BenchmarkController.h>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onLogTest(QString log);
    void onAllTestsEnded();
    void onProgressTotal(int);
    void onProgressTestValue(int);
    void onProgressTestMaximum(int);
    void onProgressTestInfinite();
    void onCurrentStepLabel(QString);
    void onTestResult(QString, int);

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;
    BenchmarkController *ctrl_;
    QDateTime testStart_;
    QDateTime testEnd_;
};

#endif // MAINWINDOW_H
