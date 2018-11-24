CPPFLAGS=-std=c++11 -I/rtmidi-3.0.0 -Wall -D__LINUX_ALSA__
LIBS=-lasound -lpthread

all: NetworkMidi mServer mClient

mServer: MidiServer.o RtMidi.h
	g++ -std=c++11 mServer.cpp RtMidi.cpp -D__LINUX_ALSA__ MidiServer.o -o mServer -lasound -lpthread
	
mClient: MidiClient.o
	g++ -std=c++11 mClient.cpp -D__LINUX_ALSA__ MidiClient.o -o mClient

NetworkMidi: MidiProbe.o RtMidi.h
	g++ -std=c++11 NetworkMidi.cpp RtMidi.cpp -D__LINUX_ALSA__ MidiProbe.o -o NetworkMidi -lasound -lpthread
	
MidiProbe.o: MidiProbe.h MidiProbe.cpp RtMidi.h
	g++ $(CPPFLAGS) -c MidiProbe.cpp RtMidi.cpp -lasound -lpthread
	
MidiServer.o: MidiServer.h MidiServer.cpp RtMidi.h
	g++ $(CPPFLAGS) -c MidiServer.cpp RtMidi.cpp -lasound -lpthread
	
MidiClient.o: MidiClient.h MidiClient.cpp
	g++ $(CPPFLAGS) -c MidiClient.cpp
