#ifndef LOGSEDIT_H
#define LOGSEDIT_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>

class LogsEdit : public QWidget
{
    Q_OBJECT
public:
    explicit LogsEdit(QWidget *parent = nullptr);
    QString readLogs();

signals:

public slots:
    void appendText(QString);
    void appendLog(QString);
    void appendErr(QString);

private:
    QPlainTextEdit content;
};

#endif // LOGSEDIT_H
