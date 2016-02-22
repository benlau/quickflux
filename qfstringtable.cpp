#include <QtCore>
#include <QMetaObject>
#include "qfstringtable.h"

/*!
  \qmltype StringTable
  \brief Defines a string table

  StringTable is a object with properties equeal to its key name. Once it's construction is completed,
  it will search all of its string property. If it is a string type and not assigned to any value,
  it will set its value by its name. It can be used to create ActionTypes.qml in QuickFlux Application.

Example

\code

StringTable {

    // It will be set to "customField1" in Component.onCompleted callback.
    property string customField1;

    // Since it is already assigned a value, StringTable will not modify this property.
    property string customField2 : "value";

}

\endcode


 */

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
