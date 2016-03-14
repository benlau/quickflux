#ifndef QUICKFLUXUNITTESTS_H
#define QUICKFLUXUNITTESTS_H

#include <QObject>

class QuickFluxUnitTests : public QObject
{
    Q_OBJECT

public:
    QuickFluxUnitTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void instance();
    void singletonObject();
};

#endif // QUICKFLUXUNITTESTS_H
