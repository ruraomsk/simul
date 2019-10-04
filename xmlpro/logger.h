#ifndef LOGGINGCATEGORIES_H
#define LOGGINGCATEGORIES_H

#include <QLoggingCategory>
#include <QFile>
#include <QScopedPointer>
#include <QDateTime>
#include <QTextStream>
#include <QMutex>

extern QScopedPointer<QFile> m_logFile;
extern QTextStream out;
#define USE_LOGGER QScopedPointer<QFile> Logger::m_logFile; \
        QTextStream Logger::out; \
        QMutex Logger::mutex; \
        QString Logger::path; \
        QDateTime Logger::nowDate;

#define FINISH_LOG Logger::out.flush();
class Logger
{
public:
    Logger(QString path);
static QScopedPointer<QFile> m_logFile;
static void messageHandler(QtMsgType type,const QMessageLogContext &context,const QString &msg);
static QTextStream out;
static QMutex mutex;
static QString path;
static QDateTime nowDate;
};

#endif // LOGGINGCATEGORIES_H
