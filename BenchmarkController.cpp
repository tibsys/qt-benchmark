#include "BenchmarkController.h"
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QThread>
#include <QRunnable>
#include <QImage>
#include <QThreadPool>
#include "MaClasse.h"

#define INSTANCIATION_MAX 400000
#define COPY_SIZE 10000
#define COMPUTATION_MAX 1000000
#define FUNCTIONS_CALL_MAX 1000000
#define RUNLOOP_CALL_MAX 1000000
#define THREADS_COUNT 100

QByteArray refData_;

BenchmarkController::BenchmarkController(QObject *parent)
    : QObject(parent)
{
    //Initialize reference data for copy
    for(int i = 0 ; i < COPY_SIZE ; refData_[i++] = 'a');
}

int BenchmarkController::durationInMilliSeconds(QDateTime &start, QDateTime &end) const
{
    int inMs = end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch();
    return inMs;
}

int BenchmarkController::durationInSeconds(QDateTime &start, QDateTime &end) const
{
    return durationInSeconds(start, end)/1000;
}

void BenchmarkController::startAllTests()
{
    canContinue_ = true;
    computationCount_ = 0;
    copyCount_ = 0;
    functionsCallsCount_ = 0;
    QTimer::singleShot(0, this, SLOT(doTestInstanciations()));
}

void BenchmarkController::doTestInstanciations()
{
    instanciationsList_.clear();
    emit signalProgressTestInfinite();
    emit signalStepLabel(tr("Instanciations"));
    emit signalProgressTotal(1);
    emit signalLogTest(tr("Démarrage des tests d'instanciation : %1 chaines d'une longueur de 1 000 caractères").arg(INSTANCIATION_MAX));
    QTimer::singleShot(0, this, SLOT(internalTestInstanciations()));
    instanciationsTestStart_ = QDateTime::currentDateTime();
}

void BenchmarkController::doTestDestruction()
{
    emit signalStepLabel("Destruction d'objets");
    emit signalProgressTotal(2);
    emit signalLogTest(tr("Destruction des %1 chaines de caractères").arg(INSTANCIATION_MAX));
    QTimer::singleShot(0, this, SLOT(internalTestDestruction()));
    destructionTestStart_ = QDateTime::currentDateTime();
}

void BenchmarkController::doTestCopy()
{
    emit signalStepLabel(tr("Copie d'objets"));
    emit signalProgressTotal(3);
    emit signalLogTest(tr("Copie de %1 objets (deep-copy)").arg(INSTANCIATION_MAX));
    copyTestStart_ = QDateTime::currentDateTime();
    QTimer::singleShot(0, this, SLOT(internalTestCopy()));
}

void BenchmarkController::doTestComputation()
{
    emit signalStepLabel(tr("Calculs divers"));
    emit signalProgressTotal(4);
    emit signalLogTest(tr("Calculs divers"));
    computationTestStart_ = QDateTime::currentDateTime();
    QTimer::singleShot(0, this, SLOT(internalTestComputation()));
}

void BenchmarkController::doTestGraphics()
{
    //Rien pour le moment
}

void BenchmarkController::doTestFunctionsCalls()
{
    emit signalStepLabel(tr("Appels de fonctions"));
    emit signalProgressTotal(6);
    emit signalLogTest(tr("Appels de fonctions"));
    functionsCallsTestStart_ = QDateTime::currentDateTime();
    QTimer::singleShot(0, this, SLOT(internalTestFunctionsCalls()));
}

void BenchmarkController::doTestRunLoop()
{
    emit signalStepLabel(tr("Appels à la run-loop"));
    emit signalProgressTotal(7);
    emit signalLogTest(tr("Appels à la run-loop"));
    runLoopTestStart_ = QDateTime::currentDateTime();
    QTimer::singleShot(0, this, SLOT(internalTestRunLoop()));
}

void BenchmarkController::doTestThreads()
{
    emit signalStepLabel(tr("Calcul parallelisé"));
    emit signalProgressTotal(8);
    emit signalLogTest(tr("Calcul parallelisé (ininterruptible)"));
    threadsTestStart_ = QDateTime::currentDateTime();
    QTimer::singleShot(0, this, SLOT(internalTestThreads()));
}

void BenchmarkController::stopAllTests()
{
    canContinue_ = false;
    emit signalLogTest(tr("Arrêt des tests en cours..."));
}

/** Private functions */
void BenchmarkController::internalTestInstanciations()
{
    instanciationsList_ << new QString(1000, 'a');

    if(instanciationsList_.size() < INSTANCIATION_MAX && canContinue_ == true) {
        //emit signalProgressTestValue(instanciationsList_.size());
        QTimer::singleShot(0, this, SLOT(internalTestInstanciations()));
    } else {
        emit signalLogTest(tr("Test terminé"));
        instanciationsTestEnd_ = QDateTime::currentDateTime();

        emit signalLogTest(tr("Instanciation des objets terminée"));
        emit signalTestDuration(tr("Instanciation"), durationInMilliSeconds(instanciationsTestStart_, instanciationsTestEnd_));

        QTimer::singleShot(0, this, SLOT(doTestDestruction()));
    }
}

void BenchmarkController::internalTestCopy()
{
    copyData_ = QByteArray(refData_.constData());
    copyCount_ += 1;

    if(copyCount_ < INSTANCIATION_MAX && canContinue_ == true) {
        QTimer::singleShot(0, this, SLOT(internalTestCopy()));
    } else {
        emit signalLogTest(tr("Test terminé"));
        copyTestEnd_ = QDateTime::currentDateTime();

        emit signalLogTest(tr("Copie des objets terminée"));
        emit signalTestDuration(tr("Copie"), durationInMilliSeconds(copyTestStart_, copyTestEnd_));

        QTimer::singleShot(0, this, SLOT(doTestComputation()));
    }
}

void BenchmarkController::internalTestDestruction()
{
    QString* s = instanciationsList_.takeAt(0);
    delete s;
    if(!instanciationsList_.isEmpty()) {
        //emit signalProgressTestValue(instanciationsList_.size());
        QTimer::singleShot(0, this, SLOT(internalTestDestruction()));
    } else {
        destructionTestEnd_ = QDateTime::currentDateTime();
        emit signalLogTest(tr("Destruction des objets terminée"));        
        emit signalTestDuration(tr("Destruction"), durationInMilliSeconds(destructionTestStart_, destructionTestEnd_));
        QTimer::singleShot(0, this, SLOT(doTestCopy()));
    }
}

void BenchmarkController::internalTestComputation()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    int val = qrand() % 0xFFFFFFFFFF;
    val *= qrand() % 0xFEAB;
    qreal val2 = qSqrt(val);
    val2 *= qExp(3.141592654);
    float f = val2/3.141592654f;
    f /= 1.1f;

    if(computationCount_++ < COMPUTATION_MAX && canContinue_) {
        QTimer::singleShot(0, this, SLOT(internalTestComputation()));
    } else {
        emit signalLogTest(tr("Test terminé"));
        computationTestEnd_ = QDateTime::currentDateTime();

        emit signalLogTest(tr("Calculs terminés"));        
        emit signalTestDuration(tr("Calculs"), durationInMilliSeconds(computationTestStart_, computationTestEnd_));
        QTimer::singleShot(0, this, SLOT(doTestFunctionsCalls()));
    }
}

void functionCall2(MaClasse cl1, MaClasse cl2)
{
    cl1.d_ = cl2.d_; //Pour éviter que l'optimisation ne supprime cet appel de fonction
}

void functionCall3(MaClasse cl1, MaClasse cl2)
{
    cl1.f_ = cl2.f_; //Pour éviter que l'optimisation ne supprime cet appel de fonction
}

void functionCall1(MaClasse cl1, MaClasse cl2, MaClasse cl3)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    if(qrand() % 2)
        functionCall2(cl1, cl2);
    else
        functionCall3(cl2, cl3);
}

MaClasse clRef1, clRef2, clRef3;
void BenchmarkController::internalTestFunctionsCalls()
{
    functionCall1(clRef1, clRef2, clRef3);
    if(functionsCallsCount_++ < FUNCTIONS_CALL_MAX && canContinue_) {
        QTimer::singleShot(0, this, SLOT(internalTestFunctionsCalls()));
    } else {
        emit signalLogTest(tr("Test terminé"));
        functionsCallsTestEnd_ = QDateTime::currentDateTime();

        emit signalLogTest(tr("Appels de fonctions terminés"));
        emit signalTestDuration(tr("Appels de fonctions"), durationInMilliSeconds(functionsCallsTestStart_, functionsCallsTestEnd_));
        QTimer::singleShot(0, this, SLOT(doTestRunLoop()));
    }
}

QString s("Ma chaine de caractère de test, elle n'est pas très longue mais elle ne sera pas copiée...");
QByteArray a("Ma chaine de caractère de test, elle n'est pas très longue mais elle ne sera pas copiée...");
int i = 0x65FF43AB;
void BenchmarkController::internalTestRunLoop()
{
    if(runLoopCallsCount_++ < RUNLOOP_CALL_MAX && canContinue_) {
        QMetaObject::invokeMethod(this, "runLoopTestSubFunction", Q_ARG(QString, s), Q_ARG(int, i), Q_ARG(QByteArray, a));
    } else {
        emit signalLogTest(tr("Test terminé"));
        runLoopTestEnd_ = QDateTime::currentDateTime();

        emit signalLogTest(tr("Test de la run-loop terminés"));
        emit signalTestDuration(tr("Appels run-loop"), durationInMilliSeconds(runLoopTestStart_, runLoopTestEnd_));
        QTimer::singleShot(0, this, SLOT(doTestThreads()));
    }
}

void BenchmarkController::runLoopTestSubFunction(QString s, int i, QByteArray a)
{
    QString s2(s); //Copie superficielle
    s2.append('a'); //Pour éviter que la ligne du dessus ne soit supprimée par une optimisation du compilateur
    QByteArray a2(a); //Copie superficielle
    a2.append('a'); //Pour éviter que la ligne du dessus ne soit supprimée par une optimisation du compilateur
    int i2 = i; //Copie profonde
    i2 += 1; //Pour éviter que la ligne du dessus ne soit supprimée par une optimisation du compilateur

    QTimer::singleShot(0, this, SLOT(internalTestRunLoop()));
}

class TacheThreadee : public QRunnable
{
    void run() {
       /* qsrand(QDateTime::currentMSecsSinceEpoch());
        int i = qrand() % 0xffffff;
        double d = sqrt(i);
        float f = */
        /* Mandelbrot computation */
        const int NumPasses = 20;
        int pass = 0;
        QSize resultSize(1024, 768);
        int halfWidth = resultSize.width() / 2;
        int halfHeight = resultSize.height() / 2;
        double scaleFactor = 1;
        double centerX = 4000;
        double centerY = 2200;
        QImage image(resultSize, QImage::Format_RGB32);

        while (pass < NumPasses) {
            const int MaxIterations = (1 << (2 * pass + 6)) + 32;
            const int Limit = 4;
            bool allBlack = true;

            for (int y = -halfHeight; y < halfHeight; ++y) { //résolution 800x600
                uint *scanLine = reinterpret_cast<uint *>(image.scanLine(y + halfHeight));
                double ay = centerY + (y * scaleFactor);

                for (int x = -halfWidth; x < halfWidth; ++x) {
                    double ax = centerX + (x * scaleFactor);
                    double a1 = ax;
                    double b1 = ay;
                    int numIterations = 0;

                    do {
                        ++numIterations;
                        double a2 = (a1 * a1) - (b1 * b1) + ax;
                        double b2 = (2 * a1 * b1) + ay;
                        if ((a2 * a2) + (b2 * b2) > Limit)
                            break;

                        ++numIterations;
                        a1 = (a2 * a2) - (b2 * b2) + ax;
                        b1 = (2 * a2 * b2) + ay;
                        if ((a1 * a1) + (b1 * b1) > Limit)
                            break;
                    } while (numIterations < MaxIterations);

                    if (numIterations < MaxIterations) {
                        *scanLine++ = 234; //valeur arbitraire au hasard
                         allBlack = false;
                    } else {
                        *scanLine++ = 142; //valeur arbitraire au hasard
                    }
                }
            }

            if (allBlack && pass == 0) {
                pass = 4;
            } else {
                ++pass;
            }
        }
    }
};

void BenchmarkController::internalTestThreads()
{
    emit signalLogTest(tr("Démarrage de %1 threads").arg(THREADS_COUNT));
    QThreadPool::globalInstance()->setMaxThreadCount(THREADS_COUNT);
    for(int i = 0 ; i < THREADS_COUNT ; i++) {
        TacheThreadee *tache = new TacheThreadee;
        QThreadPool::globalInstance()->start(tache);
    }

    //Attend la fin des threads
    QThreadPool::globalInstance()->waitForDone();
    {
        emit signalLogTest(tr("Test terminé"));
        threadsTestEnd_ = QDateTime::currentDateTime();

        emit signalLogTest(tr("Test de threads terminés"));
        emit signalTestDuration(tr("Threads"), durationInMilliSeconds(threadsTestStart_, threadsTestEnd_));

        emit signalLogTest(tr("Tous les tests sont terminés"));
        emit signalProgressTestMaximum(100);
        emit signalProgressTestValue(100);
        emit signalTestDuration(tr("Total"), durationInMilliSeconds(instanciationsTestStart_, threadsTestEnd_));
    }
}
