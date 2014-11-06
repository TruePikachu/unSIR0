#include "spritefile.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
	if(argc!=4) {
		cout << "Usage: " << argv[0] << " <SIR0 file> <main ID> <sub ID>\n";
		return 0;
	}
	ifstream pFile(argv[1]);
	SpriteFile mySprite(pFile);
	pFile.close();
	const SpriteAnim& myAnim = mySprite.getAnimation(atoi(argv[2]),atoi(argv[3]));
	vector< SpriteAnimFrame >::const_iterator frame = myAnim.getFrames().begin();
	SpriteAnimFrame::gPalette = &mySprite.getPalette();
	for(;;) {
		cout << *frame;
		++frame;
		if(frame==myAnim.getFrames().end())
			return 0;
	}
}

