#include <iostream>
#include <QObject>
#include <SFML/Network.hpp>
#include <iomanip>

#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

class SignalHandler : public QObject {
    Q_OBJECT
public:
    SignalHandler() {}

private:
    quint64 _fileSize;
    int previousPercentage = 0;

public slots:
    void printErr(const char * err) {
        std::cerr << err << std::endl;
    }
    void printLog(const char * log) {
        std::clog << log << std::endl;
    }
    void printReceived(int percentage) {
        if (previousPercentage != percentage) {
            std::cout << "....." << percentage << "%" << std::endl;
            previousPercentage = percentage;
        }
    }
    void printSent(int percentage) {
        if (previousPercentage != percentage) {
            std::cout << "....." << percentage << "%" << std::endl;
            previousPercentage = percentage;
        }
    }
    void saveFileSize(quint64 fileSize) {
        _fileSize = fileSize;
    }
};

#endif // SIGNALHANDLER_H
