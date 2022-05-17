#include "Receiver.h"
#include "Sender.h"
#include "SignalHandler.h"
#include <fstream>

using namespace std;

void connectSender(Sender & sender, SignalHandler & handler)
{
    QObject::connect(&sender, &Sender::err, &handler, &SignalHandler::printErr);
    QObject::connect(&sender, &Sender::log, &handler, &SignalHandler::printLog);
    QObject::connect(&sender, &Sender::sent, &handler, &SignalHandler::printSent);
    QObject::connect(&sender, &Sender::saveFileSize, &handler, &SignalHandler::saveFileSize);
}

void connectReceiver(Receiver & receiver, SignalHandler & handler)
{
    QObject::connect(&receiver, &Receiver::err, &handler, &SignalHandler::printErr);
    QObject::connect(&receiver, &Receiver::log, &handler, &SignalHandler::printLog);
    QObject::connect(&receiver, &Receiver::received, &handler, &SignalHandler::printReceived);
    QObject::connect(&receiver, &Receiver::saveFileSize, &handler, &SignalHandler::saveFileSize);
    QObject::connect(&receiver, &Receiver::fileNameReceived, &receiver, &Receiver::receiveFile);
}

void displayUse()
{
    cout << "Keyshare is a file transfer program under local network" << endl;
    cout << "It has 2 modes : Sender and Receiver" << endl;
    cout << "Sender" << endl << "\tkeyshare [key] [file to transfer]" << endl;
    cout << "Receiver" << endl << "\tkeyshare [key]" << endl;
}

int main(int argc, char* argv[])
{
    SignalHandler handler;
    if (argc == 3) {
        ifstream file(argv[2]);
        if (file.fail()) {
            cerr << "File " << argv[2] << " does not exist" << endl;
            exit(EXIT_FAILURE);
        } else {
            Sender s(argv[1], argv[2]);
            connectSender(s, handler);
            s.process();
        }
    } else if(argc == 2) {
        Receiver r(argv[1]);
        connectReceiver(r, handler);
        r.process();
    } else {
        displayUse();
    }
    return EXIT_SUCCESS;
}




