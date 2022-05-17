#!/usr/bin/python3
# -*- coding: utf-8 -*-



import socket
import time
import sys
import netifaces as ni

#Getting the local ip address
ni.ifaddresses('enp0s31f6')
LocalIp = ni.ifaddresses('enp0s31f6')[ni.AF_INET][0]['addr']

#Configuring the socket to receive a UDP broadcast
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
server.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
server.bind(( "",8080)) #bind to the port

# waiting for a correct key
data = ""
Key = sys.argv[2] #code
while (data != Key):
	data, addr = server.recvfrom(1024)
	data = data.decode()

# confirming that the key is correct & sending IP and port to the receiver.
server = socket. socket(socket. AF_INET, socket. SOCK_DGRAM)
keyIsCorrect = "key is correct"
bytesToSend = str.encode(keyIsCorrect)
server.sendto(bytesToSend,(addr[0], 8081))

# sending the file by TCP
s = socket.socket()
s.bind((LocalIp, 8080)) #binding to the port
s.listen(5) #waiting for the receiver connection


conn, addr = s.accept() #Establishing the connection with the client
filename = sys.argv[1]
f = open(filename, 'rb')
bytesToSend = str.encode(filename)
conn.send(bytesToSend) #Sending the name of the file
l = f.read(1024)
while (l):
	print("sending...")
	conn.send(l)
	l = f.read(1024)

f.close() #close the file
print ('Done sending')
conn.close() #close the connection