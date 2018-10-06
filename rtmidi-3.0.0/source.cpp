
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include "RtMidi.h"
class MidiProbe {
private:
	RtMidiIn *midin;
	RtMidiOut *midout;
public:
	MidiProbe(){
		try{
			midin = new RtMidiIn();
		}
		catch(RtMidiError &error){
			error.printMessage();
			exit(EXIT_FAILURE);
		}
		try {
			midout = new RtMidiOut();
		}
		catch(RtMidiError &error){
			error.printMessage();
			exit(EXIT_FAILURE);
		}
	}
	~MidiProbe(){
		delete midin;
		delete midout;
	}
	//returns -1 if status failure
	int probePorts(){
		int inPorts = midin->getPortCount();
		int outPorts = midout->getPortCount();
		std::string currentPortName = "";
		if(inPorts == -1 || outPorts == -1){
			return -1;
		}
		std::cout << "(in: " << inPorts << ", out: " << outPorts <<")\n";
		for(int i = 0; i < inPorts; i++){
			try {
				currentPortName = midin->getPortName(i);
			}	
			catch(RtMidiError &error) {
				error.printMessage();
				return -1;
			}
			std::cout << " Input Port #" <<i<<" : "<<currentPortName << std::endl;
		}
		for(int i = 0; i < outPorts; i++) {
			try {
				currentPortName = midout->getPortName(i);
			}
			catch(RtMidiError &error) {
				error.printMessage();
				return -1;
			}
			std::cout << " Output port #" << i << " : " << currentPortName << std::endl;
		}
		return 0;
	}
	
	int bindInput(int portNum){
		midin->openPort(portNum);
		return 0;
	}
	int bindOutput(int portNum){
		midout->openPort(portNum);
		return 0;
	}
	void sendTestOutput() {
		std::vector<unsigned char> message;
		// Program change: 192, 5
		message.push_back( 192 );
		message.push_back( 5 );
		midout->sendMessage( &message );
		// Control Change: 176, 7, 100 (volume)
		message[0] = 176;
		message[1] = 7;
		message.push_back( 100 );
		midout->sendMessage( &message );
		// Note On: 144, 64, 90
		message[0] = 144;
		message[1] = 64;
		message[2] = 90;
		midout->sendMessage( &message );
		usleep(15000);
		  // Note Off: 128, 64, 40
		message[0] = 128;
		message[1] = 64;
		message[2] = 40;
		midout->sendMessage( &message );
	}
	void readInputs() {
		std::vector<unsigned char> message;
		double timeDelta = 0;
		std::string types[3] = {"type ", "key ", "velocity "};
		bool initializing = true;
		int nBytes = 0;
		
		midin->ignoreTypes(false,false,false);	
			
		std::cout << "Reading Midi Input... Ctrl+C to quit for now...\n";
		
		while(true){
			timeDelta = midin->getMessage(&message);
			if(timeDelta > 0.01 && initializing) {
				initializing = false;
				std::cout << "Connection is ready \n";
			}
			nBytes = message.size();
			for(int i=0; i < nBytes; i++){
				if(!initializing){
					std::cout << types[i%3]<< (int)message[i] << ", ";
				}
			}
			if(nBytes > 0){
				std::cout << ", time_delta " << timeDelta << std::endl;
			}
			usleep(10000);
		}
	}
};


int main(){
	MidiProbe probe;
	int status = probe.probePorts();
	std::string setting = "input";
	if(status != 0){
		std::cout << "An error occured during port probing!\nProgram must now exit\n";
		return 0;
	}
	std::cout << "current setting is: "<<setting<<'\n';
	std::cout << "Select Port #: ";
	std::cin >> status;
	if(setting == "output"){
		status = probe.bindOutput(status);
		if(status == -1){
			std::cout << "an error occured in binding port!\nProgram must now exit\n";
			return 0;
		}
		for(int i=0; i < 16; i++){
			probe.sendTestOutput();
			usleep(90000);
		}
	}
	else{
		status = probe.bindInput(status);
		if(status == -1){
			std::cout << "an error occured in binding port!\nProgram must now exit\n";
			return 0;
		}
		probe.readInputs();
	}
	return 0;
	
}
