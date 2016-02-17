#ifndef QFFILTER_H
#define QFFILTER_H

#include <QObject>
#include <QQmlParserStatus>
#include <QJSValue>

// Filter represents a filter rule in AppListener

class QFFilter : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QFFilter(QObject *parent = 0);

    QString type() const;

    void setType(const QString &type);

signals:
    void dispatched(QString type, QJSValue message);

    void typeChanged();

protected:
    void classBegin();
    void componentComplete();

private slots:
    void filter(QString type, QJSValue message);

private:
    QString m_type;
};

#endif // QFFILTER_H
