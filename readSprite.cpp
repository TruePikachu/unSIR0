#include "spriteio.hpp"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc,char *argv[]) {
	if(argc!=2) {
		cout << "Usage: " << argv[0] << " <SIR0 file>\n";
		return 0;
	}
	ifstream pFile(argv[1]);
	SpriteIO mySprite(pFile);
	pFile.close();
	cout << mySprite << endl;
	return 0;
}
