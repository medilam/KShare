#ifndef SENDERTHREAD_H
#define SENDERTHREAD_H

#include <QThread>
#include "Sender.h"

class SenderThread : public QThread
{
    Q_OBJECT
public:
    explicit SenderThread(QObject *parent = nullptr);
    void run() override;
    void setKey(QString);
    void setFileName(QString);

private:
    QString _key;
    QString _fileName;
    quint64 _fileSize;

signals:
    void sent(QString);
    void sentProgressBar(int);
    void log(const char *);
    void err(const char *);
    void saveFileSize(quint64);

public slots:
    void sentHandler(int,quint64);
    void logHandler(const char *);
    void errHandler(const char *);
    void saveFileSizeHandler(quint64);

};

#endif // SENDERTHREAD_H
