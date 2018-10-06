CPPFLAGS=-std=c++11 -I/rtmidi-3.0.0 -Wall -D__LINUX_ALSA__
LIBS=-lasound -lpthread

all: NetworkMidi

NetworkMidi: MidiProbe.o RtMidi.h
	g++ -std=c++11 NetworkMidi.cpp RtMidi.cpp -D__LINUX_ALSA__ MidiProbe.o -o NetworkMidi -lasound -lpthread
	
MidiProbe.o: MidiProbe.h MidiProbe.cpp RtMidi.h
	g++ $(CPPFLAGS) -c MidiProbe.cpp RtMidi.cpp -lasound -lpthread
	
