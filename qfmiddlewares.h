#ifndef QFMIDDLEWARES_H
#define QFMIDDLEWARES_H

#include <QQuickItem>
#include <QPointer>
#include <qfactioncreator.h>

class QFMiddlewares : public QQuickItem
{
    Q_OBJECT
public:
    QFMiddlewares();

signals:

public slots:

    void apply(QObject* source);

    void next(int senderId, QString type, QJSValue message);

protected:
    void classBegin();
    void componentComplete();

private slots:

private:
    void setup(QFActionCreator* creator , QFDispatcher* dispatcher);
    void registerNextFunction();

    QPointer<QQmlEngine> m_engine;

    QPointer<QFActionCreator> m_actionCreator;
    QPointer<QFDispatcher> m_dispatcher;
    QJSValue invoke;

};

#endif // QFMIDDLEWARES_H
