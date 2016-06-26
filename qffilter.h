#ifndef QFFILTER_H
#define QFFILTER_H

#include <QObject>
#include <QQmlParserStatus>
#include <QJSValue>
#include <QVariant>

// Filter represents a filter rule in AppListener

class QFFilter : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QStringList types READ types WRITE setTypes NOTIFY typesChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QFFilter(QObject *parent = 0);

    QString type() const;

    void setType(const QString &type);

    QStringList types() const;

    void setTypes(const QStringList &types);

signals:
    void dispatched(QString type, QJSValue message);

    void typeChanged();

    void typesChanged();

protected:
    void classBegin();
    void componentComplete();

private slots:
    void filter(QString type, QJSValue message);
    void filter(QString type, QVariant message);

private:
    QStringList m_types;
};

#endif // QFFILTER_H
