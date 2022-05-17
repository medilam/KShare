#include "SenderThread.h"

SenderThread::SenderThread(QObject *parent) : QThread(parent) {}

void SenderThread::setKey(QString key)
{
    _key = key;
}

void SenderThread::setFileName(QString fileName)
{
    _fileName = fileName;
}

void SenderThread::run()
{
    Sender s(_key.toStdString().c_str(), _fileName.toStdString().c_str());

    connect(&s, &Sender::log, this, &SenderThread::logHandler);
    connect(&s,&Sender::err, this, &SenderThread::errHandler);
    connect(&s, &Sender::saveFileSize, this, &SenderThread::saveFileSizeHandler);
    connect(&s, &Sender::sent, this, &SenderThread::sentHandler);

    s.process();
}

void SenderThread::logHandler(const char * msg)
{
    emit log(msg);
}

void SenderThread::errHandler(const char * msg)
{
    emit err(msg);
}

void SenderThread::sentHandler(int prc, quint64 oct)
{
    emit sentProgressBar(prc);
    emit sent("Sending..." + QString::number(prc) +"%(" + QString::number(oct)+ "/" + QString::number(_fileSize) +"Bytes).");
}

void SenderThread::saveFileSizeHandler(quint64 fileSize)
{
    _fileSize = fileSize;
    emit saveFileSize(fileSize);
}
