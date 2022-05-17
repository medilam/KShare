#include "LogsEdit.h"
#include <QDateTime>

LogsEdit::LogsEdit(QWidget *parent) : QWidget(parent)
{

}

QString LogsEdit::readLogs()
{
    return content.toPlainText();
}

void LogsEdit::appendText(QString text)
{
    content.appendPlainText(QDateTime::currentDateTime().toString() + " : " + text);
}

void LogsEdit::appendLog(QString log)
{
    content.appendPlainText(QDateTime::currentDateTime().toString() + " LOG: " + log);
}

void LogsEdit::appendErr(QString err)
{
    content.appendPlainText(QDateTime::currentDateTime().toString() + " ERR: " + err);
}
