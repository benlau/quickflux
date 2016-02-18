#include <QtCore>
#include <QMetaObject>
#include "qfstringtable.h"

QFStringTable::QFStringTable(QObject *parent) : QObject(parent)
{

}

void QFStringTable::classBegin()
{

}

void QFStringTable::componentComplete()
{
    const QMetaObject* meta = metaObject();

    int count = meta->propertyCount();
    for (int i = 0 ; i < count ; i++) {
        const QMetaProperty p = meta->property(i);
        QString name(p.name());
        if (p.type() != QVariant::String ||
            name == "objectName") {
            continue;
        }

        QVariant v = property(p.name());
        if (!v.isNull()) {
            continue;
        }

        setProperty(p.name(), name);
    }

}
