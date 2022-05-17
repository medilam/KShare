#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QStackedWidget>
#include "LogsEdit.h"
#include "ReceiverThread.h"
#include "SenderThread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

public slots:
    void switchToReceive();
    void switchToSend();
    void waitForFile();
    void updateDropArea();
    void retrieveFile();
    void manageReceiveButton();
    void manageClearReceiveButton();
    void displayReceived(int prc);
    void browse();
    void aboutKeyshare();
    void showLogs();
    void saveFileName(char * fileName);

signals:
    void readyToReceiveFile(char *);

private:
    Ui::MainWindow * ui;
    LogsEdit * logs;

};

#endif // MAINWINDOW_H
