#include <QtCore>
#include <QVariantMap>
#include "qfhydrate.h"
#include "qfobject.h"
#include "qfstore.h"
#include <functional>

static QVariantMap dehydrator(QObject* source);

static auto dehydratorFunction = [](const QStringList& ignoreList) -> std::function<QVariantMap (QObject *)> {

    return [=](QObject* source) {
        QVariantMap dest;
        const QMetaObject* meta = source->metaObject();

        for (int i = 0 ; i < meta->propertyCount(); i++) {
            const QMetaProperty property = meta->property(i);
            const char* name = property.name();
            QString stringName = name;

            if (ignoreList.indexOf(stringName) >= 0) {
                continue;
            }

            QVariant value = source->property(name);

            if (value.canConvert<QObject*>()) {
                QObject* object = value.value<QObject*>();
                if (!object) {
                    continue;
                }
                value = dehydrator(object);
            }
            dest[stringName] = value;
        }
        return dest;
    };
};

static auto dehydrateQObject = dehydratorFunction(QStringList() << "parent" << "objectName");
static auto dehydrateQFObject = dehydratorFunction(QStringList() << "parent" << "objectName" << "children");
static auto dehydrateQFStore = dehydratorFunction(QStringList() << "parent" << "objectName" << "children" << "bindSource" << "redispatchTargets" << "filterFunctionEnabled");

/// Default dehydrator function
static QVariantMap dehydrator(QObject* source) {
    QVariantMap ret;
    if (qobject_cast<QFStore*>(source)) {
        ret = dehydrateQFStore(source);
    } else if (qobject_cast<QFObject*>(source)) {
        ret = dehydrateQFObject(source);
    } else {
        ret = dehydrateQObject(source);
    }
    return ret;
}

/*!
   \qmltype Hydrate
   \inqmlmodule QuickFlux

\code
import QuickFlux 1.1
\endcode

Hydrate provides an interface to rehydrate / hydrate a Store component. Rehydration and dehydration are just another words for deserialize and serialize. It could be used to convert Store into JSON object, and vice versa.

Remarks: Hydrate supports any QObject based type as the target of deserialize and serialize.

\code
Hydrate.rehydrate(store, {
  value1: 1,
  value2: 2.0,
  value3: "",
  value4: {
    subValue1: 1
  }
});

var data = Hydrate.dehydrate(MainStore);
console.log(JSON.stringify(data));
\endcode

It is added since Quick Flux 1.1

*/

QFHydrate::QFHydrate(QObject *parent) : QObject(parent)
{

}

/*!
  \qmlmethod Hydrate::rehydrate(target, source)

Deserialize data from source and write to target object.

\code
Hydrate.rehydrate(store, {
  value1: 1,
  value2: 2.0,
  value3: "",
  value4: {
    subValue1: 1
  }
\endcode

 */

void QFHydrate::rehydrate(QObject *dest, const QVariantMap &source)
{
    const QMetaObject* meta = dest->metaObject();

    QMap<QString,QVariant>::const_iterator iter = source.begin();
    while (iter != source.end()) {
        QByteArray key = iter.key().toLocal8Bit();

        int index = meta->indexOfProperty(key.constData());
        if (index < 0) {
            qWarning() << QString("Hydrate.rehydrate: %1 property is not existed").arg(iter.key());
            iter++;
            continue;
        }

        QVariant orig = dest->property(key.constData());
        QVariant value = source[iter.key()];

        if (orig.canConvert<QObject*>()) {
            if (value.type() != QVariant::Map) {
                qWarning() << QString("Hydrate.rehydrate: expect a QVariantMap property but it is not: %1");
            } else {
                rehydrate(orig.value<QObject*>(), value.toMap());
            }

        } else if (orig != value) {
            dest->setProperty(key.constData(), value);
        }

        iter++;
    }
}

/*!
  \qmlmethod Hydrate::dehydrate(object)

Serialize data from a object

\code
var data = Hydrate.dehydrate(MainStore);
console.log(JSON.stringify(data));
\endcode

 */

QVariantMap QFHydrate::dehydrate(QObject *source)
{
    return dehydrator(source);
}
