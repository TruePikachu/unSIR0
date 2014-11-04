#include "spritefile.hpp"
#include <cstdio>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
using namespace std;

std::ostream& operator<<(std::ostream&os, const SpriteFile&p) {
	os << "SpriteFile:\n";
	os << *p.palette << endl;
//	os << "Frames: (n=" << p.frames.size() << ")\n";
//	for(vector< SpriteFrame >::const_iterator it=p.frames.begin();it!=p.frames.end();++it)
//		os << *it << endl;
	return os << endl;
}

#define LoadDWORD(n,p)	uint32_t n;file.seekg(p);file.read((char*)&n,4)
#define LoadWORD(n,p)	uint16_t n;file.seekg(p);file.read((char*)&n,2)
SpriteFile::SpriteFile(std::istream&file) {
	// Load the palette
	LoadDWORD(offSpriteSubHeader,0x4);
	LoadDWORD(offSpriteFrameBlock,offSpriteSubHeader+0x4);
	LoadDWORD(offPaletteInfo,offSpriteFrameBlock+0x4);
	LoadDWORD(offPaletteData,offPaletteInfo+0x0);
	file.seekg(offPaletteData);
	palette = new SpritePalette;
	for(int i=0;i<16;i++) {
		uint32_t color;
		file.read((char*)&color,4);
		SpritePalette::Color newColor;
		newColor.r = (color >>  0) & 0xFF;
		newColor.g = (color >>  8) & 0xFF;
		newColor.b = (color >> 16) & 0xFF;
		newColor.a = (color >> 24) & 0xFF;
		palette->getColors().push_back(newColor);
	}
	// Load frames
	LoadWORD(nbFrames,offSpriteFrameBlock+0xE);
	// TODO
}

SpriteFile::~SpriteFile() {
	delete palette;
}

//////////

std::ostream& operator<<(std::ostream&os, const SpritePalette&p) {
	char buffer[64];
	os << "SpritePalette (" << p.colors.size() << " entries):\n";
	for(int8_t i=0;i<p.colors.size();i++) {
		sprintf(buffer,"R=%1$03u G=%2$03u B=%3$03u A=%4$03u  (#%1$02X%2$02X%3$02X %4$02X)",
				p.colors[i].r,
				p.colors[i].g,
				p.colors[i].b,
				p.colors[i].a);
		os << "0x" << (char)((i>9)?('A'-10+i):('0'+i)) << ": " << buffer << endl;
	}
	return os;
}

std::vector< SpritePalette::Color >& SpritePalette::getColors() {
	return colors;
}

//////////

SpriteFrame::~SpriteFrame() {
	delete[] data;
}
