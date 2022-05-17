#ifndef KSHARE_RECEIVER_H
#define KSHARE_RECEIVER_H

#include <QObject>
#include <SFML/Network.hpp>
#include <openssl/sha.h>
#include <fstream>

using namespace sf;
using namespace std;

#define HASH_BUFFER_SIZE 100
#define BUFFER_SIZE 4096
//#define TCP_PORT 7070
#define BROADCAST_PORT 8080

class Receiver : public QObject {
    Q_OBJECT
public:
    Receiver();
    Receiver(const char * key) : _key(key) {
    }
    bool hashFunction(void *input, unsigned int lenght, unsigned char * output);
    void sendHash();
    int connectTCP();
    void receiveFileName();
    //void receiveFile(ofstream *outfile);
    bool confirmReceivedHash(const char *);
    void process();
    void receiveOtherCoordinates(char *);
    void receiveTcpPort();
    void concat3Args( char * result, const char*, const char*, const char *);
    void synchronize();

signals:
    void fileNameReceived(char * fileName);
    void received(int,quint64);
    void log(const char *);
    void err(const char *);
    void saveFileSize(quint64);

public slots:
    void receiveFile(char *);

private:
    int tcp_port;
    unsigned short sender_port;
    const char * _key;
    Uint16 _file_name_size = 0;
    sf::TcpSocket socket;
    sf::IpAddress sender_addr;
};

#endif //KSHARE_RECEIVER_H
