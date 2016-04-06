#ifndef QUICKFLUXUNITTESTS_H
#define QUICKFLUXUNITTESTS_H

#include <QObject>

class QuickFluxUnitTests : public QObject
{
    Q_OBJECT

public:
    QuickFluxUnitTests();

signals:
    void dummySignal(int v1);
    void dummySignal(int v1,int v2);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void instance();

    void singletonObject();

    void signalProxy();

    void dispatch_qvariant();

    void keyTable();
};

#endif // QUICKFLUXUNITTESTS_H
