#include "MidiProbe.h"
#include "rtmidi-3.0.0/RtMidi.h"


MidiProbe::MidiProbe(){
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
	MidiProbe::~MidiProbe(){
	if(midin != nullptr){
		if(midin->isPortOpen()){
			midin->closePort();
		}
	}
	if(midout != nullptr){
		if(midout->isPortOpen()){
			midout->closePort();
		}
	}
	delete midin;
	delete midout;
}
//returns -1 if status failure
int MidiProbe::probePorts(){
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

int MidiProbe::bindInput(int portNum){
	if(midin->isPortOpen()){
		midin->closePort();
	}
	midin->openPort(portNum);
	return 0;
}
int MidiProbe::bindOutput(int portNum){
	if(midout->isPortOpen()){
		midout->closePort();	
	}
	midout->openPort(portNum);
	return 0;
}
void MidiProbe::sendTestOutput() {
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
void printInputCallback( double deltatime, std::vector< unsigned char > *message, void *userData ) {
	static std::string types[3] = {"type ", "key ", "velocity "};
	unsigned int nBytes = message->size();
	for ( unsigned int i=0; i<nBytes; i++ )
	std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
	if ( nBytes > 0 )
	std::cout << "stamp = " << deltatime << std::endl;
}
void MidiProbe::readInputs() {
	midin->setCallback(&printInputCallback);
	midin->ignoreTypes(false,false,false);	
	std::cout << "Reading Midi Input... Press <enter> to stop listening...\n";
	char input;
	std::cin.get(input);
}
