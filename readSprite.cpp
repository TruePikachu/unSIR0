#include "spritefile.hpp"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc,char *argv[]) {
	if(argc!=2) {
		cout << "Usage: " << argv[0] << " <SIR0 file>\n";
		return 0;
	}
	ifstream pFile(argv[1]);
	SpriteFile mySprite(pFile);
	pFile.close();
	cout << mySprite << endl;
	return 0;
}
