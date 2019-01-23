#include <QCoreApplication>
#include <QQmlEngine>
#include "messagelogger.h"

static QStringList s_messages;

static void logFunc(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(type);
    Q_UNUSED(context);
    Q_UNUSED(msg);

    MessageLogger::globalInstance()->log(msg);

    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "%s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
}

MessageLogger::MessageLogger(QObject *parent) : QObject(parent)
{
}

void MessageLogger::install()
{
    qInstallMessageHandler(logFunc);
}

void MessageLogger::log(const QString &message)
{
    s_messages << message;
}

QStringList MessageLogger::messages() const
{
    return s_messages;
}

void MessageLogger::setMessages(const QStringList &messages)
{
    s_messages = messages;
}

MessageLogger *MessageLogger::globalInstance()
{
    static MessageLogger* instance = nullptr;
    if (!instance) {
        instance = new MessageLogger(QCoreApplication::instance());
    }
    return instance;
}

void MessageLogger::clear()
{
    s_messages.clear();
}

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    MessageLogger* logger = new MessageLogger(engine);
    return logger;
}

static void init() {
    qmlRegisterSingletonType<MessageLogger>("MessageLogger", 1, 0, "MessageLogger", provider);
}

Q_COREAPP_STARTUP_FUNCTION(init)
