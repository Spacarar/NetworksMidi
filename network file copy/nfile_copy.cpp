#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const int BLOCK_SIZE = 4096;
//are we waiting for a connection, connected and awaiting input, connected and transfering data
typedef enum STATE {WAITING_INPUT, WAITING_CONNECTION, CONNECTED_INPUT, CONNECTED_TRANSFER, QUIT } State;

void error(const char *msg){
	perror(msg);
	exit(1);
}



int main(int argc, char **argv){
	const unsigned int buffer_size= BLOCK_SIZE;
	FILE* inFile = new FILE(); //file received from client
	string inFileName = "testCopy_default";
	State runningState = WAITING_INPUT;
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[buffer_size] = {0};
	int portno = 0;
	
	//create socket file descriptor
	if( (server_fd = socket(AF_INET, SOCK_STREAM, 0) ) == 0) {
		error("server socket creation failure");
	}
	//ask user for port number
	while(runningState == WAITING_INPUT){
		cout << "Port Number > 2000: ";
		cin >> portno;
		if(portno < 2000){
			cout << "error determing port: "<< portno <<endl;
		} else {
			runningState = WAITING_CONNECTION;
		}
	}
	//attach the socket to the port
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		error("error setting socket");
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( portno );
	
	//bind the port
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
		error("error binding port");
	}
	cout << "port bound succesfully. awaiting connection..." <<endl;
	while(runningState == WAITING_CONNECTION){
		if(listen(server_fd, 3) < 0){
			error("Error listening for connection");
		}
		if( (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
			error("error accepting new connection");
		} else {
			runningState = CONNECTED_INPUT;
		}
	}
	cout << "new connection established from: " <<new_socket<<endl;
	send(new_socket, "Hello client!\n", 14, 0);
	
	//when a file is going to be transfered the word file will be scanned
	bool fileNext = false;
	while(true){
		//set buffer back to 0 to avoid errors
		memset(buffer, 0, buffer_size);
		valread = read(new_socket, buffer, buffer_size);
		cout <<valread<<": "<< buffer << endl;
		if(!valread){
			cout << "lost connection!"<<endl<<endl<<"lost connection!"<<endl;
			return 0;
		}
		
		
		cout << "fileNext: "<<fileNext<<endl;
		if(valread == 4 && runningState == CONNECTED_INPUT){
			if(buffer[0] == 'f' && buffer[1] == 'i' && buffer[2] == 'l' && buffer[3] == 'e'){
					fileNext = true;
					continue;
			}
		}
		if(fileNext && runningState == CONNECTED_INPUT){
			cout << "open for transfer"<<endl;
			inFileName = "";
			for(int i = 0; i < valread -1 && i < buffer_size; i++){
				inFileName.push_back(buffer[i]);
			}
			inFileName = inFileName + "_copy";
			runningState = CONNECTED_TRANSFER;
			try {
				inFile = fopen(inFileName.c_str(),"wb");
				continue;
			}
			catch (exception& e) {
				cout << "Error opening file"<<endl;
				return -1;
			}
		}
		if(runningState == CONNECTED_TRANSFER) {
			cout << "connected transfer"<<endl;
			if(valread == 3 && buffer[0] == '!' && buffer[1] == '!' && buffer[2] == '!'){
				runningState = CONNECTED_INPUT;
				fclose(inFile);
				cout << "FINISHED WRITING FILE?!?"<<endl;
				continue;
			}
			else{
				fwrite(buffer, 1, valread, inFile);
				continue;
			}
		}
	}
	return 0;
}
