#include <QtCore>
#include <QMetaObject>
#include "qffilter.h"

QFFilter::QFFilter(QObject *parent) : QObject(parent)
{

}

QString QFFilter::type() const
{
    return m_type;
}

void QFFilter::setType(const QString &type)
{
    m_type = type;
    emit typeChanged();
}

void QFFilter::classBegin()
{

}

void QFFilter::componentComplete()
{
    QObject* object = parent();

    if (!object) {
        qDebug() << "Filter - Disabled due to missing parent. ";
        return;
    }

    connect(object,SIGNAL(dispatched(QString,QJSValue)),
            this,SLOT(filter(QString,QJSValue)));

}

void QFFilter::filter(QString type, QJSValue message)
{
    if (type == m_type) {
        emit dispatched(type, message);
    }
}
