#include "qfappscriptgroup.h"

QFAppScriptGroup::QFAppScriptGroup(QQuickItem* parent) : QQuickItem(parent)
{

}

QJSValue QFAppScriptGroup::scripts() const
{
    return m_scripts;
}

void QFAppScriptGroup::setScripts(const QJSValue &scripts)
{
    for (int i = 0 ; i < objects.count() ; i++) {
        if (objects.at(i).data()) {
            objects.at(i)->disconnect(this);
        }
    }

    objects.clear();
    m_scripts = scripts;

    if (!scripts.isArray()) {
        qWarning() << "AppScriptGroup: Invalid scripts property";
        return;
    }

    int count = scripts.property("length").toInt();

    for (int i = 0 ; i < count ; i++) {
        QJSValue item = scripts.property(i);

        if (!item.isQObject()) {
            qWarning() << "AppScriptGroup: Invalid scripts property";
            continue;
        }

        QFAppScript* object = qobject_cast<QFAppScript*>(item.toQObject());

        if (!object) {
            qWarning() << "AppScriptGroup: Invalid scripts property";
            continue;
        }
        objects << object;
        connect(object,SIGNAL(started()),
                this,SLOT(onStarted()));
    }

    emit scriptsChanged();
}

void QFAppScriptGroup::exitAll()
{
    for (int i = 0 ; i < objects.count() ; i++) {
        if (objects.at(i).data()) {
            objects.at(i)->exit();
        }
    }
}

void QFAppScriptGroup::onStarted()
{
    QFAppScript* source = qobject_cast<QFAppScript*>(sender());

    for (int i = 0 ; i < objects.count() ; i++) {
        QPointer<QFAppScript> object = objects.at(i);
        if (object.isNull())
            continue;

        if (object.data() != source) {
            object->exit();
        }
    }

}

