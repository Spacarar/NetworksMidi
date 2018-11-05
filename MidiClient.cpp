#include "MidiClient.h"

MidiClient::MidiClient(string ip_addr, int portno){
	this->ip_address = ip_addr;
	this->serv_portno = portno;
	this->clearBuffer();
}
void MidiClient::clearBuffer(){
	memset(this->buffer, 0, this->buffer_size);
}
int MidiClient::establishConnection(){
	this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0){
		cout << "error creating socket file descriptor"<<endl;
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_pot = htons(serv_portno);
	if(inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) < 0){
		cout << "connection to server failed"<<endl;
		return -1;
	}
	cout << "established connection to " <<this->ip_address<<endl;
	return 0;
}

int MidiClient::sendGreeting(){
	valread = read(sock_fd, buffer, buffer_size);
	cout << valread << endl;
	cout << buffer<<endl;
	send(sock_fd, "hello from client\n", 18, 0);
	
