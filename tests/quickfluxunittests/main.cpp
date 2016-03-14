#include <QString>
#include <QtTest>
#include "testrunner.h"
#include "quickfluxunittests.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);

    TestRunner runner;
    runner.add<QuickFluxUnitTests>();
    runner.add(QString(SRCDIR) + "/qmltests");

    bool error = runner.exec(app.arguments());

    if (!error) {
        qWarning() << "All test cases passed!";
    }

    return error;
}
