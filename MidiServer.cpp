#include "MidiServer.h"

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

MidiServer::MidiServer(){
	this->client_fd = this->server_fd = this->valread = 0;
	this->configurePort();
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
}
	
