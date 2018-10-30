#include<string>
#include<iostream>
#include<fstream>

using namespace std;
const int BLOCK_SIZE = 4096;

int main(int argc, char **argv) {
	const unsigned int buffer_size = BLOCK_SIZE;
	size_t size;
	string sourceName, destName;
	char buf[buffer_size];
	FILE* source = new FILE();
	FILE* dest = new FILE();
	bool inputNeeded = true;
	while (inputNeeded) {
		cout << "enter an input file name: ";
		getline(cin, sourceName);
		try {
			fopen_s(&source, sourceName.c_str(),"rb");
		}
		catch (exception& e) {
			continue;
		}
		cout << "\n enter an output file name: ";
		getline(cin, destName);
		try {
			fopen_s(&dest, destName.c_str(), "wb");
		}
		catch (exception& e) {
			continue;
		}
		inputNeeded = false;
	}
	while (size = fread(buf, 1, buffer_size, source)) {
		fwrite(buf, 1, size, dest);
	}
	fclose(source);
	fclose(dest);
	return 0;
}
