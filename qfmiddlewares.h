#ifndef QFMIDDLEWARES_H
#define QFMIDDLEWARES_H

#include <QQuickItem>
#include <QPointer>
#include <qfactioncreator.h>

class QFMiddlewares : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* applyTarget READ applyTarget WRITE setApplyTarget NOTIFY applyTargetChanged)
public:
    QFMiddlewares();

    QObject *applyTarget() const;
    void setApplyTarget(QObject *applyTarget);

signals:
    void applyTargetChanged();

public slots:

    void apply(QObject* source);

    void next(int senderId, QString type, QJSValue message);

protected:
    void classBegin();
    void componentComplete();

private slots:
    void setup();

private:

    QPointer<QQmlEngine> m_engine;

    QPointer<QFActionCreator> m_actionCreator;
    QPointer<QFDispatcher> m_dispatcher;
    QJSValue invoke;

    QPointer<QObject> m_applyTarget;

};

#endif // QFMIDDLEWARES_H
