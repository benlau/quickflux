#ifndef MESSAGELOGGER_H
#define MESSAGELOGGER_H

#include <QtGlobal>
#include <QObject>

class MessageLogger : public QObject
{
    Q_OBJECT
public:
    explicit MessageLogger(QObject *parent = nullptr);

    void log(const QString& message);

    void setMessages(const QStringList &messages);

    static MessageLogger* globalInstance();

public slots:
    void install();
    void clear();
    QStringList messages() const;

private:
};

#endif // MESSAGELOGGER_H
