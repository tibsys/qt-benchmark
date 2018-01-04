#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), ctrl_(new BenchmarkController(this))
{
    ui->setupUi(this);

    connect(ctrl_, &BenchmarkController::signalLogTest, this, &MainWindow::onLogTest);
    connect(ctrl_, &BenchmarkController::signalProgressTestValue, this, &MainWindow::onProgressTestValue);
    connect(ctrl_, &BenchmarkController::signalProgressTestInfinite, this, &MainWindow::onProgressTestInfinite);
    connect(ctrl_, &BenchmarkController::signalProgressTestMaximum, this, &MainWindow::onProgressTestMaximum);
    connect(ctrl_, &BenchmarkController::signalStepLabel, this, &MainWindow::onCurrentStepLabel);
    connect(ctrl_, &BenchmarkController::signalProgressTotal, this, &MainWindow::onProgressTotal);
    connect(ctrl_, &BenchmarkController::signalTestDuration, this, &MainWindow::onTestResult);

    ui->progressTotal->setMaximum(8);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLogTest(QString log)
{
    ui->logTest->append(QString("%1 %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).arg(log));
}

void MainWindow::onProgressTotal(int value) {
    ui->progressTotal->setValue(value);
}

void MainWindow::onProgressTestValue(int value)
{
    ui->progressTest->setValue(value);
}

void MainWindow::onProgressTestMaximum(int value)
{
    ui->progressTest->setMaximum(value);
}

void MainWindow::onProgressTestInfinite()
{
    ui->progressTest->setValue(0);
    ui->progressTest->setMaximum(0);
    ui->progressTest->setMinimum(0);
}

void MainWindow::on_btnStart_clicked()
{
    onLogTest(tr("Démarrage du benchmark"));
    ctrl_->startAllTests();
    testStart_ = QDateTime::currentDateTime();
}

void MainWindow::on_btnStop_clicked()
{
    ctrl_->stopAllTests();
}

void MainWindow::onCurrentStepLabel(QString stepLabel)
{
    ui->lblCurrentStep->setText(stepLabel);
}

void MainWindow::onAllTestsEnded()
{
    onLogTest(tr("Tous les tests sont terminés"));

    //Computation of tests duration
}

void MainWindow::onTestResult(QString testName, int result)
{
    ui->logResults->append(QString("%1 : %2 ms").arg(testName).arg(result));
}
