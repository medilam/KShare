#ifndef KSHARE_SENDER_H
#define KSHARE_SENDER_H

#include <cstring>
#include <SFML/Network.hpp>
#include <QObject>
#include <openssl/sha.h>

#define HASH_BUFFER_SIZE 100
#define LOG_BUFFER_SIZE 80
//#define TCP_PORT 7070
#define BUFFER_SIZE 4096
#define BROADCAST_PORT 8080

class Sender : public QObject {
    Q_OBJECT
public:
    Sender();
    Sender(const char * key, const char * file_addr = nullptr) {
        _key = key;
        _file_addr = file_addr;
    }
    void receiveConnectionRequest(char hashReceived[SHA256_DIGEST_LENGTH]);
    int connection_TCP();
    void sendFileName();
    void loadAndSendFile();
    void sendHash();
    void sendPortNumber(const char *);
    void process();
    bool confirmReceivedHash(const char *);
    bool hashFunction(void *input, unsigned int length, unsigned char * output);
    void concat3Args( char * result, const char*, const char*, const char *);
    void synchronize();

signals:
    void sent(int, quint64);
    void log(const char *);
    void err(const char *);
    void saveFileSize(quint64);

private:
    const char * _key;
    int tcp_port = 7070;
    unsigned short receiver_port;
    std::string _file_addr;
    sf::IpAddress receiver_addr;
    sf::TcpListener listener;
    sf::TcpSocket client;
};

#endif //KSHARE_SENDER_H
