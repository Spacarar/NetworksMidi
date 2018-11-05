//Midi Client
//Receives midi information from the midi server
//converts the midi information to a synthesized sound
#pragma once

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

class MidiClient{
	const unsigned int buffer_size = 4096;
	unsigned char buffer[buffer_size];
	int serv_portno;
	string ip_address;
	int sock_fd;
	struct sockaddr_in serv_addr;
	
	int establishConnection();
	//receive hello from server, send hello from client
	//receive ports from server, send # from client
	//receive midi info from server, send response of info
	//play midi data received as it is received.
	int sendGreeting();
	void clearBuffer();
public:
	MidiClient(string ip_addr="127.0.0.1", int portno=4445);

};
