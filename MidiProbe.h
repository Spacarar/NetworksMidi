#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include "rtmidi-3.0.0/RtMidi.h"
	
class MidiProbe {
private:
	RtMidiIn *midin;
	RtMidiOut *midout;
public:
	MidiProbe();
	~MidiProbe();
	//returns -1 if status failure
	int probePorts();
	
	int bindInput(int portNum);
	int bindOutput(int portNum);
	void sendTestOutput();
	void readInputs();
};
