#ifndef BENCHMARKCONTROLLER_H
#define BENCHMARKCONTROLLER_H

#include <QObject>
#include <QDateTime>

class BenchmarkController : public QObject
{
    Q_OBJECT
public:
    explicit BenchmarkController(QObject *parent = 0);

public slots:
    void startAllTests();
    void doTestInstanciations();
    void doTestCopy();
    void doTestDestruction();
    void doTestComputation();
    void doTestGraphics();
    void doTestFunctionsCalls();
    void doTestRunLoop();
    void doTestThreads();
    void stopAllTests();

private slots:
    void internalTestInstanciations();
    void internalTestCopy();
    void internalTestDestruction();
    void internalTestComputation();
    void internalTestFunctionsCalls();
    void internalTestRunLoop();
    void runLoopTestSubFunction(QString s, int i, QByteArray a);
    void internalTestThreads();

signals:
    void signalLogTest(QString log);
    void signalProgressTestValue(int value);
    void signalProgressTotal(int value);
    void signalProgressTestInfinite();
    void signalProgressTestMaximum(int value);
    void signalStepLabel(QString);
    void signalTestDuration(QString testName, int duration);

private:
    inline int durationInSeconds(QDateTime&, QDateTime&) const;
    inline int durationInMilliSeconds(QDateTime&, QDateTime&) const;
    bool canContinue_ = true;

    // Instanciations test
    QDateTime instanciationsTestStart_;
    QDateTime instanciationsTestEnd_;
    QList<QString*> instanciationsList_;

    //Destruction test
    QDateTime destructionTestStart_;
    QDateTime destructionTestEnd_;

    //Copy test
    QDateTime copyTestStart_;
    QDateTime copyTestEnd_;
    QByteArray copyData_;
    int copyCount_ = 0;

    //Computation test
    QDateTime computationTestStart_;
    QDateTime computationTestEnd_;
    int computationCount_ = 0;

    //Functions calls test
    QDateTime functionsCallsTestStart_;
    QDateTime functionsCallsTestEnd_;
    int functionsCallsCount_= 0;

    //Run-loop test
    QDateTime runLoopTestStart_;
    QDateTime runLoopTestEnd_;
    int runLoopCallsCount_ = 0;

    //Threads test
    QDateTime threadsTestStart_;
    QDateTime threadsTestEnd_;
};

#endif // BENCHMARKCONTROLLER_H
