/// QML Type Registration Helper
#include <QtQml>
#include "qfappdispatcher.h"
#include "qfapplistener.h"
#include "qfappscript.h"
#include "qfapplistenergroup.h"
#include "qfappscriptgroup.h"
#include "priv/qfappscriptrunnable.h"

static QObject *appDispatcherProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QFAppDispatcher* object = new QFAppDispatcher();

    return object;
}

class QFQmlTypes {

public:
    QFQmlTypes() {
        qmlRegisterSingletonType<QFAppDispatcher>("QuickFlux", 1, 0, "AppDispatcher", appDispatcherProvider);
        qmlRegisterType<QFAppListener>("QuickFlux", 1, 0, "AppListener");
        qmlRegisterType<QFAppScript>("QuickFlux", 1, 0, "AppScript");
        qmlRegisterType<QFAppListenerGroup>("QuickFlux", 1, 0, "AppListenerGroup");
        qmlRegisterType<QFAppScriptGroup>("QuickFlux", 1, 0, "AppScriptGroup");
        qmlRegisterType<QFAppScriptRunnable>();
    }
};

static QFQmlTypes registerHelper;
