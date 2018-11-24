#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include "MidiClient.h"

using namespace std;

int main(int argc, char** argv){
	cout << "client started" <<endl;
	MidiClient* mClient = new MidiClient();
	mClient->startClient();
	return 0;
}
