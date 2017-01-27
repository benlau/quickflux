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

    void actionCreator_genKeyTable();

    void actionCreator_changeDispatcher();

    void dispatcherHook();

    void loading();
    void loading_data();

};

#endif // QUICKFLUXUNITTESTS_H
