#include "Receiver.h"
#include <cstring>
#include <QFileInfo>
#include <QTime>

using namespace std;
using namespace sf;

void Receiver::process()
{
    sendHash();
    char receivedHash[SHA256_DIGEST_LENGTH];
    bool hashMatched = false;
    while (!hashMatched) {
        receiveOtherCoordinates(receivedHash);
        hashMatched = confirmReceivedHash(receivedHash);
    }
    if (hashMatched) {
        emit log("hash matched !");
        synchronize();
        receiveTcpPort();
        if (connectTCP() == 0 ) { 
            receiveFileName();
        } else {
            emit log("Please try again");
        }
    }
}

void Receiver::sendHash()
{
    char buffer[HASH_BUFFER_SIZE];
    concat3Args(buffer, IpAddress::getLocalAddress().toString().c_str(), to_string(BROADCAST_PORT).c_str(), _key);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    if (!hashFunction(buffer, strlen(buffer), hash)) {
        emit err("error while calling hashFunction.");
    } else {

        UdpSocket s;
        s.send(hash, SHA256_DIGEST_LENGTH, IpAddress::Broadcast, BROADCAST_PORT);
    }
}

void Receiver::synchronize()
{
    UdpSocket s;
    string t = "t";
    s.bind(sender_port);
    s.send(t.c_str(), 2, sender_addr, BROADCAST_PORT + 2);
    s.unbind();
}

void Receiver::receiveOtherCoordinates(char receivedHash[SHA256_DIGEST_LENGTH])
{
    UdpSocket s;
    s.bind(BROADCAST_PORT + 1);
    size_t key_size;
    while(s.receive(receivedHash, SHA256_DIGEST_LENGTH, key_size, sender_addr, sender_port) != Socket::Done);
    s.unbind();
    emit log("sender coordinates received");
}

void Receiver::receiveTcpPort() {
    UdpSocket so;
    char tcpPortChar[5];
    so.bind(BROADCAST_PORT + 3);
    size_t key_size;
    while(so.receive(tcpPortChar, 5, key_size, sender_addr, sender_port) != Socket::Done);
    so.unbind();
    tcp_port = stoi(tcpPortChar);
    emit log("tcp port number received");
}

bool Receiver::confirmReceivedHash(const char * receivedHash)
{
    char buffer[HASH_BUFFER_SIZE];
    concat3Args(buffer, sender_addr.toString().c_str(), to_string(BROADCAST_PORT+1).c_str(), _key);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    if (!hashFunction(buffer, strlen(buffer), hash)) {
        emit err("error while calling hashFunction");
        return false;
    }
    return memcmp(hash, receivedHash, SHA256_DIGEST_LENGTH) == 0;
}

int Receiver::connectTCP()
{
    Socket::Status statusTCP;
    /*statusTCP = socket.connect(sender_addr,tcp_port);
    if (statusTCP == Socket::Done) {
        emit log("Client connected to the server!");
    } else {
        emit log("Client didn't connect to the server!");
        return 1;
    }
    return 0;*/
    do {
        statusTCP = socket.connect(sender_addr,tcp_port);
        if (statusTCP == Socket::Done) {
            emit log("Client connected to the server!");
        } else {
            emit log("Client didn't connect to the server! Retrying...");
        }
    } while (statusTCP != Socket::Done);
    return 0;
}

void Receiver::receiveFileName()
{
    //Receiving the size of the file name
    Packet sizePacket;
    if (socket.receive(sizePacket) == Socket::Done) {
        sizePacket >> _file_name_size;
    }

    //Receiving the file name and open it
    size_t receivedBytes;
    char buffer[BUFFER_SIZE];
    ofstream outfile;
    if (socket.receive(buffer, _file_name_size, receivedBytes) == Socket::Done) {
        emit fileNameReceived(buffer);
    }
}

void Receiver::receiveFile( char * fileName)
{
    QFileInfo fileSystem(fileName);
    if (fileSystem.exists()) {
        emit log("File already exists : renaming...");
        string now = QTime::currentTime().toString().append(".").toStdString();
        now.erase(remove(now.begin(), now.end(), ':'), now.end());
        concat3Args(fileName, fileSystem.completeBaseName().toStdString().c_str(), now.c_str(), fileSystem.completeSuffix().toStdString().c_str());
    }
    ofstream outfile;

    outfile.open(fileName, ios::out | ios::binary);

    Packet sizePacket;

    sizePacket << (Uint16)5;
    socket.send(sizePacket);

    size_t receivedBytes;
    char buffer[BUFFER_SIZE];

    //Receiving the file size
    Uint64 fileSize = 1;
    sizePacket.clear();
    if (socket.receive(sizePacket) == Socket::Done) {
        sizePacket >> fileSize;
        emit saveFileSize(fileSize);
    }

    //Writing the file
    quint64 totalBytesReceived = 0;
    int percentage;
    while (socket.receive(buffer, BUFFER_SIZE, receivedBytes) == Socket::Done) {
        outfile.write(buffer, receivedBytes);
        fill(buffer, buffer + BUFFER_SIZE, 0);
        totalBytesReceived += receivedBytes;
        percentage = static_cast<int>((float)totalBytesReceived/(float)fileSize * 100);
        emit received(percentage, totalBytesReceived);
        if (totalBytesReceived == fileSize) {
            break;
        }
    }
    socket.disconnect();
    outfile.close();
}

bool Receiver::hashFunction(void *input, unsigned int length, unsigned char * output)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return false;

    if(!SHA256_Final(output, &context))
        return false;

    return true;
}

void Receiver::concat3Args(char* dest, const char* part1, const char* part2, const char* part3)
{
    strcpy(dest, part1);
    strcat(dest, part2);
    strcat(dest, part3);
}

