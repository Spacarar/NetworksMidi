//Midi Client
//Receives midi information from the midi server
//converts the midi information to a synthesized sound
#pragma once

//these old c libraries need to be included to accomplish some of the tasks required
#include <cstdlib>
#include <cstring>
#include <cstdio>

//these things are std c++
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

//these things are external libraries
//no need for RtMidi because midi data is sent from server.


using namespace std;

class MidiClient{
	static const unsigned int buffer_size = 4096;
	unsigned char buffer[buffer_size];
	int serv_portno;
	int valread; //when reading from server this tells how many bytes
	string ip_address;
	int sock_fd;
	struct sockaddr_in serv_addr;
	
	int establishConnection();
	//receive hello from server, send hello from client
	//receive ports from server, send # from client
	//receive midi info from server, send response of info
	//play midi data received as it is received.
	int sendGreeting();
	int configureMidiPort();
	int listenToMidiInput();
	void clearBuffer();
	bool isSuccess(int length); //send valread to isSuccess and it will read buffer
public:
	MidiClient(string ip_addr="127.0.0.1", int portno=4445);
	void startClient();

};
