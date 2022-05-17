#include "Sender.h"
#include <SFML/System/FileInputStream.hpp>
#include <fstream>
#include <sstream>


using namespace std;
using namespace sf;

void Sender::process()
{
    char receivedHash[SHA256_DIGEST_LENGTH];
    bool hashMatched = false;
    while (!hashMatched) {
        receiveConnectionRequest(receivedHash);
        hashMatched = confirmReceivedHash(receivedHash);
    }
    if(hashMatched) { 
        emit log("hash matched !");
        sendHash();
        synchronize();
        if (connection_TCP() == 0) {
            sendFileName();
            loadAndSendFile();
        } else {
            emit err("Connection TCP failed. Please try again.");
            listener.close();
        }
    } else {
        emit log("hash don't match");
    }
}

void Sender::receiveConnectionRequest(char receivedHash[SHA256_DIGEST_LENGTH])
{
    UdpSocket s;
    s.bind(BROADCAST_PORT);
    size_t key_size;
    while(s.receive(receivedHash, SHA256_DIGEST_LENGTH, key_size, receiver_addr, receiver_port) != Socket::Done);
    s.unbind();
}

bool Sender::confirmReceivedHash(const char * hashReceived)
{
    char buffer[100];
    concat3Args(buffer, receiver_addr.toString().c_str(), to_string(BROADCAST_PORT).c_str(), _key);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    if (!hashFunction(buffer, strlen(buffer), hash)) {
        emit err("error while calling hashFunction.");
        return false;
    }
    return memcmp(hash, hashReceived,SHA256_DIGEST_LENGTH) == 0;
}

void Sender::sendHash()
{
    char buffer[HASH_BUFFER_SIZE];
    concat3Args(buffer, IpAddress::getLocalAddress().toString().c_str(), to_string(BROADCAST_PORT+1).c_str(), _key);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    if (!hashFunction(buffer, strlen(buffer), hash)) {
        emit err("error while calling hashFunction.");
    } else {
        UdpSocket s;
        s.bind(receiver_port);
        s.send(hash, SHA256_DIGEST_LENGTH, receiver_addr, BROADCAST_PORT + 1);
        s.unbind();
    }
}

void Sender::synchronize()
{
    UdpSocket s;
    char  t[2];
    s.bind(BROADCAST_PORT + 2);
    size_t key_size;
    while(s.receive(t, 2, key_size, receiver_addr, receiver_port) != Socket::Done);
    s.unbind();

}

void Sender::sendPortNumber(const char* portNumber) {
    UdpSocket s;
    s.bind(receiver_port);
    s.send(portNumber, 5, receiver_addr, BROADCAST_PORT + 3);
    s.unbind();
}

int Sender::connection_TCP()
{
    while (listener.listen(tcp_port) != Socket::Done) {
        tcp_port += 5;
        emit err("TCP Listen failed, trying again...");
    }
    sendPortNumber(to_string(tcp_port).c_str());
    do {
        if (listener.accept(client) != Socket::Done)
        {
            emit err("TCP Accept failed");
        } else {
            if (client.getRemoteAddress() != receiver_addr) {
                char buffer[LOG_BUFFER_SIZE];
                concat3Args(buffer, "Warning: ", client.getRemoteAddress().toString().c_str(), " is trying to connect!");
                emit log(buffer);
                client.disconnect();
            }
        }
    } while (client.getRemoteAddress() != receiver_addr);
    emit log("Client connected to the server!");
    
    return 0;
}

void Sender::sendFileName() {
    string file_name;
    istringstream file_addr_stream(_file_addr);
    while(std::getline(file_addr_stream,file_name, '/')) {
    } //get the file name

    //retrieve file name size and put it in a packet
    Uint16 _file_name_size = file_name.size();
    Packet sizePacket;
    sizePacket << _file_name_size;

    //sending the file name size
    client.send(sizePacket);
    //sending the file name
    client.send(file_name.c_str(), _file_name_size);
}

void Sender::loadAndSendFile()
{
    Packet sizePacket;
    Uint16 five = 0;
    while (five != 5) {
        if (client.receive(sizePacket) == Socket::Done) {
            sizePacket >> five;
        }
    }
    sizePacket.clear();
    //getting the file to send and its size
    ifstream fileToSend(_file_addr.c_str(), ios::in | ios::binary | ios::ate);
    Uint64 fileSize = fileToSend.tellg();
    fileToSend.seekg(0, ios::beg);
    //sending the file size
    sizePacket.clear();
    sizePacket << fileSize;
    client.send(sizePacket);
    emit saveFileSize(fileSize);
    //sending the file
    char buffer[BUFFER_SIZE];
    quint64 sentBytes = 0;
    int pourcentage;
    while(fileToSend.read(buffer, BUFFER_SIZE))
    {
        client.send(buffer, BUFFER_SIZE); // Send the packet
        sentBytes += BUFFER_SIZE;
        fill(buffer, buffer + BUFFER_SIZE, 0); // Clear the buffer
        pourcentage = (float)sentBytes/(float)fileSize * 100;
        emit sent(pourcentage, sentBytes);
    }
    //send the last bytes.
    client.send(buffer, fileSize % BUFFER_SIZE);
    sentBytes += fileSize % BUFFER_SIZE;
    emit sent((float)sentBytes/fileSize * 100, sentBytes);
    client.disconnect();
    listener.close();
}

bool Sender::hashFunction(void *input, unsigned int length, unsigned char * output)
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

void Sender::concat3Args(char* dest, const char* part1, const char* part2, const char* part3)
{
    strcpy(dest, part1);
    strcat(dest, part2);
    strcat(dest, part3);
}
