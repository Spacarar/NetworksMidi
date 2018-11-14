#include "MidiServer.h"

//CONSTRUCTORS
MidiServer::MidiServer(){
	this->client_fd = this->server_fd = this->valread = 0;
	this->configurePort();
	this->clearBuffer();
}
MidiServer::MidiServer(int portNum){
	this->client_fd = this->server_fd = this->valread = 0;
	if(portNum < 1024){
		cout << "port must be standard user access (# > 1024)"<<endl;
		this->configurePort();
	}
	else{
		this->portno = portNum;
	}
	this->clearBuffer();
}


//SERVER CONFIGURATION
int MidiServer::configurePort(){
	cout << "defaulting to port 4445"<<endl;
	this->portno = 4445;
	return 0;
}
int MidiServer::bindServer(){
	int opt = 1;
	//set server file descriptor
	this->server_fd = socket(AF_INET, SOCK_STERAM, 0);
	if(this->server_fd == 0){
		cout << "error setting file descriptor"<<endl;
		return -1;
	}
	//attack socket to port
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		cout << "error attaching socket to port"<<endl;
		return -1;
	}
	
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->portno);
	if(bind(this->server_fd, (struct sockaddr *)&(this->address), sizeof(this->address))<0){
		cout << "error binding port to address"<<endl;
		return -1;
	}
	cout << "port: "<<this->portno<<"  bound to server"<<endl;
	return 0;
}


void MidiServer:clearBuffer(){
	memset(this->buffer, 0, this->buffer_size);
}

int MidiServer::awaitConnection(){
	return listen(sock_fd, 5);
}

int MidiServer::sendGreeting() {
	if(!sock_fd){
		cout << "sock_fd not yet defined?" <<endl;
		return -1;
	}
	this->clearBuffer();
	send(sock_fd, "Welcome to the Network Midi Server by Shawn Pacarar", 51, 0)
	valread = read(sock_fd, buffer, buffer_size)
	if(!valread){
		cout << "nothing here, client likely got disconnected" <<endl;
		return -1;
	}
	cout << buffer <<endl;
	return 0;
}
	
int MidiServer::sendMidiPortInformation(){
	//probe midi ports
	//construct a message to send where size is known
	//send message to client
	//attempt listening to port
	//if success send success
	//otherwise send over errors
}

	
