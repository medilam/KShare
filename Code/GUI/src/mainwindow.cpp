#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "droparea.h"
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logs(new LogsEdit(this))
{
    ui->setupUi(this);
    ui->statusbar->showMessage(tr("Welcome to Keyshare!"));
    logs->appendText("Welcome to Keyshare!");
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->startWidget));
    connect(ui->dropArea, &DropArea::changed, this, &MainWindow::updateDropArea);
    connect(ui->receiveKeyEdit, &QLineEdit::textChanged, this, &MainWindow::manageReceiveButton);
    connect(ui->sendKeyEdit, &QLineEdit::textChanged, this, &MainWindow::updateDropArea);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browse);
    connect(ui->clearReceiveButton, &QPushButton::clicked, this, &MainWindow::manageClearReceiveButton);
    connect(ui->action_About, &QAction::triggered, this, &MainWindow::aboutKeyshare);
    connect(ui->action_Logs, &QAction::triggered, this, &MainWindow::showLogs);
}



void MainWindow::switchToReceive()
{
    ui->receiveModeButton->setEnabled(false);
    ui->sendModeButton->setEnabled(true);
    ui->receiveModeButton->setFlat(true);
    ui->sendModeButton->setFlat(false);
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->receiveWidget));
    ui->action_Open->setEnabled(false);
}

void MainWindow::switchToSend()
{
    ui->sendModeButton->setEnabled(false);
    ui->receiveModeButton->setEnabled(true);
    ui->sendModeButton->setFlat(true);
    ui->receiveModeButton->setFlat(false);
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->sendWidget));
    ui->action_Open->setEnabled(true);
}

void MainWindow::waitForFile()
{
    if(ui->receiveKeyEdit->text().isEmpty()) {
        logs->appendText("Please enter the key");
        ui->statusbar->showMessage(tr("Please enter a key"));
    } else {
        ui->receiveLabel->setText(tr("waiting for the file..."));
        ui->statusbar->showMessage(tr("waiting for the file..."));
        logs->appendText("waiting for the file...");
        ReceiverThread * receiver = new ReceiverThread(this);
        connect(receiver, &ReceiverThread::log, logs, &LogsEdit::appendLog);
        connect(receiver, &ReceiverThread::err, logs, &LogsEdit::appendErr);
        connect(receiver, &ReceiverThread::received, logs, &LogsEdit::appendText);
        connect(receiver, &ReceiverThread::fileNameReceived, this, &MainWindow::saveFileName);
        connect(this, &MainWindow::readyToReceiveFile, receiver, &ReceiverThread::readyToReceiveFileHandler);
        connect(receiver, &ReceiverThread::receivedProgressBar, ui->receiveProgress, &QProgressBar::setValue);
        connect(receiver, &ReceiverThread::receivedProgressBar, this, &MainWindow::displayReceived);
        receiver->setKey(ui->receiveKeyEdit->text());
        receiver->start();
    }
}

void MainWindow::updateDropArea()
{
    if (ui->dropArea->getStatus() == FULL){
        ui->clearSendButton->setEnabled(true);
        if (!ui->sendKeyEdit->text().isEmpty() ) {
            ui->sendButton->setEnabled(true);
        } else {
            ui->sendButton->setDisabled(true);
        }
    } else {
        ui->clearSendButton->setDisabled(true);
        ui->sendButton->setDisabled(true);
    }
    ui->sendProgress->setValue(ui->sendProgress->minimum());
}

void MainWindow::retrieveFile()
{
    if(ui->sendKeyEdit->text().isEmpty()) {
        logs->appendText("Please enter a key");
        ui->statusbar->showMessage(tr("Please enter a key"));
    } else {
        QString filename = ui->dropArea->getText().mid(7,ui->dropArea->getText().length()-9);
        SenderThread *sender = new SenderThread(this);
        
        connect(sender, &SenderThread::log, logs, &LogsEdit::appendLog);
        connect(sender, &SenderThread::err, logs, &LogsEdit::appendErr);
        connect(sender, &SenderThread::sent, logs, &LogsEdit::appendText);
        connect(sender, &SenderThread::sentProgressBar, ui->sendProgress, &QProgressBar::setValue);
        
        sender->setKey(ui->sendKeyEdit->text());
        sender->setFileName(filename);
        sender->start();
        ui->statusbar->showMessage(tr("Sending file..."));
    }

}

void MainWindow::manageReceiveButton()
{
    if (ui->receiveKeyEdit->text().isEmpty()) {
        ui->receiveButton->setDisabled(true);
    } else {
        ui->receiveButton->setEnabled(true);
    }
}

void MainWindow::manageClearReceiveButton()
{
    ui->receiveLabel->setText(tr("Enter a key and click Receive"));
    ui->receiveProgress->setValue(ui->receiveProgress->minimum());
    ui->clearReceiveButton->setEnabled(false);
}

void MainWindow::displayReceived(int prc)
{
    if (prc == ui->receiveProgress->maximum()) {
        ui->receiveLabel->setText(tr("File received!"));
        ui->statusbar->showMessage(tr("File received!"));
        logs->appendText("File received!");
        ui->clearReceiveButton->setEnabled(true);
    }
}

void MainWindow::browse()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a file"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (!fileName.isEmpty()) {
        ui->dropArea->setStatus(FULL);
        QPixmap pixmapLogo(":/icons/file");
        ui->dropArea->setPixmap(pixmapLogo);
        ui->dropArea->setText("file://" + fileName + "\r\r");
        ui->sendProgress->setValue(ui->sendProgress->minimum());
        if (!ui->sendKeyEdit->text().isEmpty()) {
            ui->sendButton->setEnabled(true);
        }
        if (!ui->clearSendButton->isEnabled()) {
            ui->clearSendButton->setEnabled(true);
        }
    }
}

void MainWindow::aboutKeyshare()
{
    QMessageBox about(this);
    about.setWindowTitle(tr("About Keyshare"));
    about.setIconPixmap(QPixmap(":/logo/logo_pivot"));
    QString content(tr("Software designed for a 2nd year project at the Ensicaen in 2020/2021.\n\n"));
    content.append(tr("Project team :\nHervault Jules\nLamhamdi Mehdi\nPeigné Steven\n\n"));
    content.append(tr("Client: Mr Sébastien Fourey\n\n"));
    content.append(tr("Keyshare is a local sharing program destined to every kind of user."));
    about.setText(content);
    about.exec();
}

void MainWindow::showLogs()
{
    QMessageBox logsDisplay(this);
    logsDisplay.setWindowTitle(tr("Logs"));
    logsDisplay.setIconPixmap(QPixmap(":/logo/logo_s"));
    logsDisplay.setText(tr("Click on Show details... to read logs"));
    logsDisplay.setDetailedText(logs->readLogs());
    logsDisplay.exec();
}

void MainWindow::saveFileName(char * _fileName)
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Choose where to put received data"),
                                                        _fileName,
                                                        tr("Files (*)"));
    if(fileName.isEmpty()){
            return;
    } else {
        QByteArray ba = fileName.toLocal8Bit();
        char *fileName2 = ba.data();
        emit readyToReceiveFile(fileName2);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete logs;
}
