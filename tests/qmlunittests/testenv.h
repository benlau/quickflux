#ifndef TESTENV_H
#define TESTENV_H

#include <QObject>
#include <QImage>

class TestEnv : public QObject
{
    Q_OBJECT
public:
    explicit TestEnv(QObject *parent = 0);

signals:

public slots:
    QImage createQImage(int width,int height);
};

#endif // TESTENV_H
