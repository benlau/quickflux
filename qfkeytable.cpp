#include <QtCore>
#include <QMetaObject>
#include "qfkeytable.h"

/*!
  \qmltype KeyTable
  \brief Defines a key table

  KeyTable is an object with properties equal to its key name. Once it's construction is completed,
  it will search all of its string property. If it is a string type and not assigned to any value,
  it will set its value by its name. It can be used to create ActionTypes.qml in QuickFlux Application.

Example

\code

KeyTable {

    // It will be set to "customField1" in Component.onCompleted callback.
    property string customField1;

    // Since it is already assigned a value, KeyTable will not modify this property.
    property string customField2 : "value";

}

\endcode


 */

static QMap<int,QString> createTypes() {
    QMap<int,QString> types;
    types[QVariant::String] = "QString";
    types[QVariant::Int] = "int";
    types[QVariant::Double] = "qreal";

    return types;
}

QFKeyTable::QFKeyTable(QObject *parent) : QObject(parent)
{

}

QString QFKeyTable::genHeaderFile(const QString& className)
{

    QStringList header;

    header << "#pragma once";
    header << "#include <QString>\n";
    header << QString("class %1 {\n").arg(className);
    header << "public:\n";

    const QMetaObject* meta = metaObject();

    QMap<int,QString> types = createTypes();

    int count = meta->propertyCount();
    for (int i = 0 ; i < count ; i++) {
        const QMetaProperty p = meta->property(i);

        QString name(p.name());

        if (name == "objectName") {
            continue;
        }

        if (types.contains(p.type())) {
            header << QString("    static %2 %1;\n").arg(name).arg(types[p.type()]);
        }

    }

    header << "};\n";

    return header.join("\n");
}

QString QFKeyTable::genSourceFile(const QString &className, const QString &headerFile)
{
    QMap<int,QString> types = createTypes();

    QStringList source;

    source << QString("#include \"%1\"\n").arg(headerFile);

    const QMetaObject* meta = metaObject();

    int count = meta->propertyCount();
    for (int i = 0 ; i < count ; i++) {
        const QMetaProperty p = meta->property(i);
        QString name(p.name());
        if (name == "objectName") {
            continue;
        }

        if (types.contains(p.type())) {
            QVariant v = property(p.name());

            if (p.type() == QVariant::String) {
                source << QString("%4 %1::%2 = \"%3\";\n")
                          .arg(className)
                          .arg(p.name())
                          .arg(v.toString())
                          .arg(types[p.type()]);

            } else {

                source << QString("%4 %1::%2 = %3;\n")
                          .arg(className)
                          .arg(p.name())
                          .arg(v.toString())
                          .arg(types[p.type()]);
            }
        }
    }

    return source.join("\n");
}

void QFKeyTable::classBegin()
{

}

void QFKeyTable::componentComplete()
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
