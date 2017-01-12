#ifndef QFMIDDLEWARES_H
#define QFMIDDLEWARES_H

#include <QQuickItem>

class QFMiddlewares : public QQuickItem
{
    Q_OBJECT
public:
    QFMiddlewares();

signals:

public slots:

protected:
    void classBegin();
    void componentComplete();

private:
    void registerNext();

    QQmlEngine* m_engine;

};

#endif // QFMIDDLEWARES_H
