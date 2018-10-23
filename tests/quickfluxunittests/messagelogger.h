#ifndef MESSAGELOGGER_H
#define MESSAGELOGGER_H

#include <QtGlobal>
#include <QObject>

class MessageLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList messages READ messages WRITE setMessages NOTIFY messagesChanged)
public:
    explicit MessageLogger(QObject *parent = nullptr);

    void log(const QString& message);

    QStringList messages() const;

    void setMessages(const QStringList &messages);

    static MessageLogger* globalInstance();

signals:
    void messagesChanged();

public slots:
    void install();
    void clear();

private:
    QStringList m_messages;
};

#endif // MESSAGELOGGER_H
