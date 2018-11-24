#include "MidiClient.h"

MidiClient::MidiClient(string ip_addr, int portno){
	this->ip_address = ip_addr;
	this->serv_portno = portno;
	this->clearBuffer();
}

void MidiClient::startClient(){
	int result = 0;
	cout << "starting client"<<endl;
	cout << "ip: "<< this->ip_address << endl;
	cout << "server port: "<< this->serv_portno <<endl;
	
	result = this->establishConnection();
	if(result < 0) return;
	cout << "connection established" <<endl;
	
	result = this->sendGreeting();
	if(result < 0) return;
	cout << "greeting sent succesfully"<<endl;
	
	result = this->configureMidiPort();
	if(result < 0) return;
	cout << "midi port successfully set" << endl;
	
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
	serv_addr.sin_port = htons(serv_portno);
	if(inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) < 0){
		cout << "connection to server failed"<<endl;
		return -1;
	}
	if( connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		cout << "Connection failed!"<<endl;
		return -1;
	}
	cout << "established connection to " <<this->ip_address<<endl;
	return 0;
}

int MidiClient::sendGreeting(){
	//upon connection server sends value
	valread = read(sock_fd, buffer, buffer_size);
	cout << valread << endl;
	cout << buffer<<endl;
	//send back our greeting to server
	send(sock_fd, "hello from client\n", 18, 0);
	//after greeting get info connection
	clearBuffer();
	valread = read(sock_fd, buffer, buffer_size);
	cout << "Midi Ports available from server"<<endl;
	cout << buffer<<endl;
	return 0;
}

int MidiClient::configureMidiPort(){
	bool configured = false;
	string userInput = "";
	while(!configured){
		cout << "#:"<<endl;
		cin >> userInput;
		send(sock_fd, userInput.c_str(), userInput.size(), 0);
		clearBuffer();
		valread = read(sock_fd, buffer, buffer_size);
		if(isSuccess(valread)){
			cout << "midi port configured" <<endl;
			configured = true;
		} else {
			cout << "error binding midi port"<<endl;
			cout << buffer <<endl;
			configured = false;
		}
			
	}
	return 0;
}

bool MidiClient::isSuccess(int length){
	if(length < 7){
		//success\n is 8 letters
		return false;
	}
	for(int i = 0; i < length && i < 7; i++){
		string success="success";
		if(this->buffer[i] == success[i]){
			cout << success[i];
		}
		else{
			cout << "\nfailing character: "<< buffer[i] <<endl;
			return false;
		}
	}
	cout <<endl;
	return true; //looks like success to me
}
