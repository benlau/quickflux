#ifndef APPVIEW_H
#define APPVIEW_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QJSValue>

class AppView : public QObject
{
    Q_OBJECT
public:
    explicit AppView(QObject *parent = 0);

    int exec();

signals:

public slots:

private slots:
    void onDispatched(QString type, QJSValue message);

private:

    QQmlApplicationEngine m_engine;

};

#endif // APPVIEW_H
