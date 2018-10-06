#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include "rtmidi-3.0.0/RtMidi.h"
#include "MidiProbe.h"


int main(){
	MidiProbe probe;
	bool keepAlive = true;
	int status = probe.probePorts();
	std::string setting = "help";
	if(status != 0){
		std::cout << "An error occured during port probing!\nProgram must now exit\n";
		return 0;
	}
	while(keepAlive) {
		if(setting == "output"){
			std::cout << "Select Port #: ";
			std::cin >> status;
			std::cin.get(); //clear state for reading input
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
		else if(setting == "input"){
			std::cout << "Select Port #: ";
			std::cin >> status;
			std::cin.get(); //clear state for reading input
			status = probe.bindInput(status);
			if(status == -1){
				std::cout << "an error occured in binding port!\nProgram must now exit\n";
				return 0;
			}
			probe.readInputs();
		}
		else if(setting == "help") {
			std::cout << "commands: input, output, probe, help, quit\n";
		}
		else if(setting == "probe") {
			int status = probe.probePorts();
			 if(status != 0){
				 std::cout << "an error occured during port probing!\nProgram must now exit\n";
				 return 0;
			 }
		 }
		 else if(setting == "quit"){
			 keepAlive = false;
			 break;
		 }
		 else{
			 std::cout << "unknown command!\n";
		 } std::cout << "cmd: ";
		std::cin >> setting;
		std::cin.get();//clear input for possible midi input
	 }
		
	std::cout << "program has finished"<<std::endl;
	return 0;
	
}
