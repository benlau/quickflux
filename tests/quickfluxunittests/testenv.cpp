#include <QtQml>
#include "testenv.h"

TestEnv::TestEnv(QObject *parent) : QObject(parent)
{

}

QImage TestEnv::createQImage(int width, int height)
{
    return QImage(width,height,QImage::Format_RGB32);
}


static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    TestEnv* object = new TestEnv();

    return object;
}

class TestEnvRegisterHelper {

public:
    TestEnvRegisterHelper() {
        qmlRegisterSingletonType<TestEnv>("QuickFlux", 1, 0, "TestEnv", provider);
    }
};

static TestEnvRegisterHelper registerHelper;


