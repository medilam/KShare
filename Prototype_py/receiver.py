#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket
import time
import sys
import netifaces as ni

#Getting the local ip address & the broadcast address
ni.ifaddresses('enp0s31f6') #the name of the network
LocalIp = ni.ifaddresses('enp0s31f6')[ni.AF_INET][0]['addr']
broadcastAddr = ni.ifaddresses('enp0s31f6')[ni.AF_INET][0]['broadcast']


#Sending the key using UDP.
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
client.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
client.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
key = sys.argv[1] #Getting the key from argv
bytesToSend = str.encode(key) #Encode the key to send it
client.sendto(bytesToSend, (broadcastAddr, 8080)) #Broadcasting the key

#Receiving the confirmation of the key and the IP address of the sender
client1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
data = ""
client1.bind((LocalIp, 8081)) #bind to the port

while (data != "key is correct"): #waiting for the confirmation
	data, addr = client1.recvfrom(1024)
	data = data.decode()

print("key sent is correct")

# receiving the file
s = socket.socket()
s.connect((addr[0], 8080)) #Connecting to the sender using TCP
fileName = s.recv(1024); #Receiving the name of the file
fileName = fileName.decode()
with open(fileName, 'wb') as f:
	while True:
		data = s.recv(1024)
		if not data:
			break
		print("Receiving...")
		f.write(data)
		
f.close() #close the file
print('Successfully get the file')
s.close() #close the connection