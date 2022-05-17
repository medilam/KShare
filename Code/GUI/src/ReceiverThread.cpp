#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(QObject *parent) : QThread(parent) {}

void ReceiverThread::setKey(QString key)
{
    _key = key;
}

void ReceiverThread::run()
{
    static Receiver receiver(_key.toStdString().c_str());
    _receiver = &receiver;
    connect(&receiver, &Receiver::log, this, &ReceiverThread::logHandler);
    connect(&receiver,&Receiver::err, this, &ReceiverThread::errHandler);
    connect(&receiver, &Receiver::saveFileSize, this, &ReceiverThread::saveFileSizeHandler);
    connect(&receiver, &Receiver::received, this, &ReceiverThread::receivedHandler);
    connect(&receiver, &Receiver::fileNameReceived, this, &ReceiverThread::fileNameReceivedHandler);
    receiver.process();
}

void ReceiverThread::logHandler(const char * msg)
{
    emit log(msg);
}

void ReceiverThread::errHandler(const char * msg)
{
    emit err(msg);
}

void ReceiverThread::receivedHandler(int prc, quint64 oct)
{
    emit receivedProgressBar(prc);
    emit received("Receiving ..." + QString::number(prc) + "%(" + QString::number(oct) + "/" + QString::number(_fileSize)+ "Bytes).");
}

void ReceiverThread::saveFileSizeHandler(quint64 fileSize)
{
    _fileSize = fileSize;
    emit saveFileSize(fileSize);
}

void ReceiverThread::readyToReceiveFileHandler(char * fileName)
{
    _receiver->receiveFile(fileName);
}


void ReceiverThread::fileNameReceivedHandler( char * fileName)
{
    emit fileNameReceived(fileName);
}
