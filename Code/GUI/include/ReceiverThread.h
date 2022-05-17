#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QThread>
#include "Receiver.h"


class ReceiverThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiverThread(QObject *parent = nullptr);
    void run() override;
    void setKey(QString);

private:
    QString _key;
    quint64 _fileSize;
    Receiver * _receiver;

signals:
    void fileNameReceived(char * fileName);
    void received(QString);
    void receivedProgressBar(int);
    void log(const char *);
    void err(const char *);
    void saveFileSize(quint64);

public slots:
    void fileNameReceivedHandler(char * fileName);
    void receivedHandler(int,quint64);
    void logHandler(const char *);
    void errHandler(const char *);
    void saveFileSizeHandler(quint64);
    void readyToReceiveFileHandler(char *);

};

#endif // RECEIVERTHREAD_H
