#include "MidiServer.h"

//CONSTRUCTORS
MidiServer::MidiServer(){
	midi = new RtMidiIn();
	this->server_fd = this->client_fd = this->valread = 0;
	this->configurePort();
	this->clearBuffer();
}
MidiServer::MidiServer(int portNum){
	midi = new RtMidiIn();
	this->server_fd = this->client_fd = this->valread = 0;
	if(portNum < 1024){
		cout << "port must be standard user access (# > 1024)"<<endl;
		this->configurePort();
	}
	else{
		this->portno = portNum;
	}
	this->clearBuffer();
}

MidiServer::~MidiServer(){
	if(this->midi != nullptr){
		if(midi->isPortOpen()){
			midi->closePort();
		}
		delete midi;
	}
}

//success message agreed upon by server/client
void MidiServer::sendSuccess(){
	if(!client_fd){
		cout << "server not yet bound attempting to send success"<<endl;
		return;
	}
	send(client_fd, "success\n", 8, 0);
}

//greeting upon new connection
int MidiServer::sendGreeting() {
	if(!server_fd){
		cout << "server_fd not yet defined?" <<endl;
		return -1;
	}
	this->clearBuffer();
	send(client_fd, "Welcome to the Network Midi Server by Shawn Pacarar", 51, 0);
	valread = read(client_fd, buffer, buffer_size);
	if(!valread){
		cout << "nothing here, client likely got disconnected" <<endl;
		return -1;
	}
	cout << buffer <<endl;
	return 0;
}


//START SERVER
//integer results will return 0 if success, -1 if failure, -2 if retry is allowed
void MidiServer::startServer(){
	int result = 0;
	cout << "starting midi server" <<endl;
	
	result = this->bindServer();
	if(result == -1) return;
	cout << "server bound and ready"<<endl;
	
	bool running = true;
	while(running){
		result = this->awaitConnection();
		if(result == -1) continue;
		cout << "connection established" <<endl;
		
		result = this->sendGreeting();
		if(result == -1)continue;
		cout << "greeting sent succesfully"<<endl;
		
		result = this->sendMidiPortInformation();
		//-2 = failure to bind port (message not understood?) retry
		//-1 = unacceptable failure (restart)
		while(result == -2){
			//just keep trying for now
			this->sendMidiPortInformation();
		}
		if(result == -1)continue;
		
		//at this step server configuration should be complete
		//the midi input port should be bound
		//the server is ready to listen to music.
		this->sendSuccess();
		cout << "midi port succesfully bound and success message sent"<<endl;
		//bool listening = true;
		readMidiInput();//continous function
	}
}

//SERVER CONFIGURATION
//this just sets port to some arbitrary default I picked
int MidiServer::configurePort(){
	cout << "defaulting to port 4445"<<endl;
	this->portno = 4445;
	return 0;
}
//create socket file descriptor and bind socket to port
int MidiServer::bindServer(){
	int opt = 1;
	//set server file descriptor
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
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


void MidiServer::clearBuffer(){
	memset(this->buffer, 0, this->buffer_size);
}

//not sure if both listen and accept should be here.
//unclear how listen exactly works
int MidiServer::awaitConnection(){
	if (listen(server_fd, 5) < 0){
		return -1;
	}
	else {
		int addrlen = sizeof(address);
		client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	}
		
	return client_fd;
}

	
int MidiServer::sendMidiPortInformation(){
	//probe midi ports
	//construct a message to send where size is known
	//send message to client
	//attempt listening to port
	//if success send success
	//otherwise send over errors
	
	int inPorts = midi->getPortCount();
	std::string currentPortName = "";
	if(inPorts == -1){
		return -1;
	}
	string message = "";
	message += "(input ports: " + to_string(inPorts)  +")\r\n";
	std::cout << "(input ports: " << inPorts <<")\n\0";
	for(int i = 0; i < inPorts; i++){
		try {
			currentPortName = midi->getPortName(i);
		}	
		catch(RtMidiError &error) {
			error.printMessage();
			return -1;
		}
		message +=  "Input Port #" + to_string(i) + " : " + currentPortName;
		std::cout << " Input Port #" <<i<<" : "<<currentPortName << std::endl;
	}
	send(client_fd, message.c_str(), message.size(), 0);
	this->clearBuffer();
	valread = read(client_fd, buffer, buffer_size);
	if(!valread){
		cout << "nothing here, client likely got disconnected" <<endl;
		return -1;
	}
	try{
		int mPort = -1;
		//1 digit response
		//offset unsigned char by 48 for integer... lame way but idk what else would work
		mPort = int(buffer[0]) - 48;
		cout << mPort<<endl;
		
		if(midi->isPortOpen()){
			midi->closePort();
		}
		
		midi->openPort(mPort);
	}
	catch(RtMidiError &e){
		e.printMessage();
		return -1;
	}
	return 0;
}

//listens on an open channel
int MidiServer::readMidiInput(){
	if(!midi->isPortOpen()){
		cout << "no midi port is open when attempting to read inputs!"<<endl;
		return -1;
	}
	if(!server_fd || !client_fd){
		cout << "server and/or client is not yet bound when attempting to read inputs!"<<endl;
		return -1;
	}
	//set callback to send server information
	midi->setCallback(&convertMidiInput, (void*)this);
	midi->ignoreTypes(false,false,false);
	cout << "reading inputs from piano and sending through callback"<<endl;
	cout << "press enter to stop reading data"<<endl;
	//listen to a midi message and fill up buffer
	char msg;
	//rtMidi way of continous read
	cin.get(msg);
	return 0;
}

//rtMidi input callback
void MidiServer::convertMidiInput(double deltatime, std::vector< unsigned char > *message, void *userData){
	MidiServer* m = (MidiServer*)userData;
	static string types[3] = {"type ", "key ", "velocity "};
	string sendData = "";
	unsigned int nBytes = message->size();
	for ( unsigned int i=0; i<nBytes; i++ ){
		cout << types[i%3] << " = " << (int)message->at(i) << ", ";
		sendData += (int)message->at(i);
		//cout << "sendData: "<<sendData<<endl;
	}
	if ( nBytes > 0 ){
		cout << "stamp = " << deltatime << std::endl;
		//cout << "sendData: "<<sendData<<endl;
		m->sendMidiInput(sendData);
	}
}

void MidiServer::sendMidiInput(string &message){
	if(!client_fd || !server_fd){
		cout << "server and/or client is not yet bound when attempting to send input to client"<<endl;
		return;
	}
	send(client_fd, message.c_str(), message.size(), 0);
} 
