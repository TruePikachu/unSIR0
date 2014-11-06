#include "spritefile.hpp"
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
	SpriteFile mySprite(pFile);
	pFile.close();
	SpriteAnimFrame::gPalette = new SpritePalette(mySprite.getPalette());
	const SpriteAnimFrame& myFrame = mySprite.getAnimation(0,1).getFrames()[0];
	cout << myFrame;
	cout << endl;
	return 0;
}
