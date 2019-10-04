#include "logger.h"


Logger::Logger(QString path)
{
    this->path=path;
    nowDate=QDateTime::currentDateTime();
    QString namefile=path+"/log"+nowDate.toString("yyyyMMdd")+".log";
    m_logFile.reset(new QFile(namefile));
    m_logFile.data()->open(QFile::Append|QFile::Text);
    out.setDevice(m_logFile.data());
    qInstallMessageHandler(Logger::messageHandler);
    qInfo()<<"Start loggin...";
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    mutex.lock();
    if(nowDate.daysTo(QDateTime::currentDateTime())!=0){
        m_logFile->close();
        nowDate=QDateTime::currentDateTime();
        QString namefile=path+"/log"+nowDate.toString("yyyyMMdd")+".log";
        m_logFile.reset(new QFile(namefile));
        m_logFile.data()->open(QFile::Append|QFile::Text);
        out.setDevice(m_logFile.data());
        qInstallMessageHandler(Logger::messageHandler);
        qInfo()<<"Continue loggin...";
    }
    out<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    switch (type) {
    case QtInfoMsg : out<<"INF ";break;
    case QtDebugMsg : out<<"DBG ";break;
    case QtWarningMsg : out<<"WRN ";break;
    case QtCriticalMsg : out<<"CRT ";break;
    case QtFatalMsg : out<<"FTL ";break;
    }
    if(type!=QtInfoMsg){
        out<<"file="<<context.file<<" line="+QString::number(context.line)+" function="+context.function+":";
    }
    out<<msg<<endl;
    out.flush();
    mutex.unlock();
}
