#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include "rtmidi-3.0.0/RtMidi.h"
#include "MidiServer.h"

int main(int argc, char** argv){
	MidiServer* mServer = new MidiServer();
	mServer->startServer();
	return 0;
}
