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
		send(sock_fd, userInput.c_str(), userInput.size());
		clearBuffer();
		valread = read(sock_fd, buffer, buffer_size);
		if(isSuccess(buffer)){
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

bool MidiClient::isSuccess(unsigned char[] &buff){
	if(buff.size() < 7){
		//success is 7 letters
		return false;
	}
	for(unsigned int i = 0; i < buff.size() || 7; i++){
		string success="success";
		if(buff[i] == success[i]){
			cout << success[i];
		}
		else{
			return false;
		}
	}
	return true; //looks like success to me
}
