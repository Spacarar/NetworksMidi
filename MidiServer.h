//Midi Server using RtMidi
//Step1 Setup & configure midi device
//step2 read inputs from midi device
//step3 listen for connecting clients
//step4 send midi data to client

#pragma once

#include <stdlib>
#include <unistd.h>
#include <sys/types>
#include <sys/socket>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>

#include "rtmidi-3.0.0/RtMidi.h"

class MidiServer{
protected:
	const unsigned int buffer_size = 4096;
	unsigned char buffer[buffer_size]; //buffer used by server to send messages
	std::vector<unsigned char> midiBuffer; //buffer used by midi to read input
	int server_fd;
	int client_fd;
	int valread;
	struct sockaddr_in address;
	int portno;
	
	RtMidiIn *midi;
	
	int configurePort();
	int bindServer();
	
	//when a server succesfully connects to a client the greeting should be sent
	//greeting says hello from server, hello from client. 
	//server then displays the connected midi devices on the server side
	//client response should be the port number they wish to listen to
	//server and client then exchange midi information and responses.
	int sendGreeting();

public:
	MidiServer();
	MidiServer(std::string ipAddress, int portNum);
	~MidiServer
};
