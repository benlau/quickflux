#include <QString>
#include <QtQuickTest/quicktest.h>
#include "testrunner.h"
#include "quickfluxunittests.h"
#include "messagelogger.h"

namespace AutoTestRegister {
    QUICK_TEST_MAIN(QuickTests)
}

int main(int argc, char *argv[])
{
    qputenv("QML_DISABLE_DISK_CACHE", "true");

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
