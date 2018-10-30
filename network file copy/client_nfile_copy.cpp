#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>


using namespace std;

const int BLOCK_SIZE = 4096;
typedef enum State {SERVER_PORT, SERVER_CONNECTION, SERVER_INPUT, FILE_TRANSFER,QUIT} State;

int main(int argc, char const *argv[]){
	const int buffer_size = BLOCK_SIZE;
	FILE* outFile = new FILE();
	string outFileName = "testFile";
	State runningState = SERVER_PORT;
	int serv_portno = 0;
	string serv_ip = "127.0.0.1";
	int sock = 0;
	int valread;
	struct sockaddr_in serv_addr;
	char buffer[buffer_size];
	
	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		cout << "socket creation error"<<endl;
		return -1;
	}
	while(runningState == SERVER_PORT){
		cout << "What port is the server running on: ";
		cin >> serv_portno;
		if(serv_portno < 0){
			cout << "could not determine port: "<<serv_portno<<endl;
		} else {
			runningState = SERVER_INPUT;
		}
	}
	//FIXME change to SERVER_CONNECTION and ask for ip address
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serv_portno);
	if(inet_pton(AF_INET, serv_ip.c_str(), &serv_addr.sin_addr)< 0){
		cout << "Connection to server failed"<<endl;
		return -1;
	}
	if( connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		cout << "Connection failed!"<<endl;
		return -1;
	}
	send(sock,"hello from client\n", 18, 0);
	cout << "type file 'file name' to send a file to the server" <<endl;
	valread = read(sock, buffer, buffer_size);
	cout << valread<<": "<<buffer<<endl;
	string hello = "";
	while(true){
		if(hello.size() >= 4 && hello[0] == 'f' && hello[1] == 'i' && hello[2] == 'l' && hello[3] == 'e'){
			cout << "hit enter"<<endl;
			cin.get();
			try {
				outFile = fopen(outFileName.c_str(),"rb");
			}
			catch (exception& e) {
				cout << "Error opening " << outFileName <<endl;
				return -1;
			}
			cout << "sending file over the wire!"<<endl;
			send(sock, outFileName.c_str(), outFileName.size(), 0);
			send(sock, "\n", 1, 0);
			usleep(2000);
			cout << "starting read"<<endl;
			while( (valread = fread(buffer, 1, buffer_size, outFile)) ){
				cout << valread<< endl;
				send(sock, buffer, valread, 0);
			}
			usleep(2000);
			send(sock, "!!!", 3, 0);
			cout << "transfer complete"<<endl;
		}
		hello = "";
		cin >> hello;
		cin.get();
		send(sock, hello.c_str(), hello.size(), 0);
	}
	
	cout << "end of connection"<<endl;
	cin.get();
	cin.get();
	
	return 0;
}
