#include "qfobject.h"

QFObject::QFObject(QObject *parent) : QObject(parent)
{

}

QQmlListProperty<QObject> QFObject::children()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this),
                                     &m_children);
#else
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this),
                                     m_children);
#endif
}
