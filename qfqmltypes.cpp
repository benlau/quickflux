/// QML Type Registration Helper
#include <QtQml>
#include "QuickFlux"
#include "qfappdispatcher.h"
#include "qfapplistener.h"
#include "qfappscript.h"
#include "qfapplistenergroup.h"
#include "qfappscriptgroup.h"
#include "priv/qfappscriptrunnable.h"
#include "qffilter.h"
#include "qfkeytable.h"
#include "qfactioncreator.h"

static QObject *appDispatcherProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QFAppDispatcher* object = new QFAppDispatcher();
    object->setEngine(engine);

    return object;
}

void registerQuickFluxQmlTypes()
{
    qmlRegisterSingletonType<QFAppDispatcher>("QuickFlux", 1, 0, "AppDispatcher", appDispatcherProvider);
    qmlRegisterType<QFAppListener>("QuickFlux", 1, 0, "AppListener");
    qmlRegisterType<QFAppScript>("QuickFlux", 1, 0, "AppScript");
    qmlRegisterType<QFAppListenerGroup>("QuickFlux", 1, 0, "AppListenerGroup");
    qmlRegisterType<QFAppScriptGroup>("QuickFlux", 1, 0, "AppScriptGroup");
    qmlRegisterType<QFAppScriptRunnable>();
    qmlRegisterType<QFFilter>("QuickFlux", 1, 0, "Filter");
    qmlRegisterType<QFKeyTable>("QuickFlux", 1, 0, "KeyTable");
    qmlRegisterType<QFActionCreator>("QuickFlux", 1, 0, "ActionCreator");
}

// Allow to disable QML types auto registration as required by #9
#ifndef QUICK_FLUX_DISABLE_AUTO_QML_REGISTER
Q_COREAPP_STARTUP_FUNCTION(registerQuickFluxQmlTypes)
#endif
