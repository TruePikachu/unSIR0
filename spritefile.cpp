#include "spritefile.hpp"
#include <cstdio>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
using namespace std;

std::ostream& operator<<(std::ostream&os, const SpriteFile&p) {
	os << "SpriteFile:\n";
	os << *p.palette << endl;
	os << "Frames: (n=" << p.frames.size() << ")\n";
	for(vector< SpriteFrame >::const_iterator it=p.frames.begin();it!=p.frames.end();++it)
		os << *it << endl;
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
		palette->getColors().push_back(newColor);
	}
	// Load frames
	LoadWORD(nbFrames,offSpriteFrameBlock+0xE);
	LoadDWORD(offFrmPtrsTable,offSpriteFrameBlock+0x0);
	for(int i=0;i<nbFrames;i++) {
		// Count the number of pixels
		typedef struct {uint32_t source;uint32_t count;uint32_t unk;} writeCmd;
		unsigned nPixels=0;
		LoadDWORD(offFrameStart,offFrmPtrsTable+4*i);

		file.seekg(offFrameStart);
		writeCmd doCmd;
		for(;;) {
			file.read((char*)&doCmd,sizeof(doCmd));
			if(doCmd.count)
				nPixels += 2*doCmd.count;
			else
				break;
		}
		SpriteFrame newFrame(nPixels);
		file.seekg(offFrameStart);
		uint8_t *writePos = newFrame.getData();
		for(;;) {
			file.read((char*)&doCmd,sizeof(doCmd));
			if(doCmd.count==0)
				break;
			if(doCmd.source) {
				off_t old = file.tellg();
				file.seekg(doCmd.source);
				for(int i=0;i<doCmd.count;i++) {
					uint8_t buf;
					file.read((char*)&buf,1);
					*(writePos++) = (buf>>0)&0xF;
					*(writePos++) = (buf>>4)&0xF;
				}
				file.seekg(old);
			} else {
				memset(writePos,0xFF,doCmd.count*2);
				writePos+=doCmd.count*2;
			}
		}
		frames.push_back(newFrame);
	}
}

SpriteFile::~SpriteFile() {
	delete palette;
}

//////////

std::ostream& operator<<(std::ostream&os, const SpritePalette&p) {
	char buffer[64];
	os << "SpritePalette (" << p.colors.size() << " entries):\n";
	for(int8_t i=0;i<p.colors.size();i++) {
		sprintf(buffer,"R=%1$03u G=%2$03u B=%3$03u  #%1$02X%2$02X%3$02X",
				p.colors[i].r,
				p.colors[i].g,
				p.colors[i].b);
		os << "0x" << (char)((i>9)?('A'-10+i):('0'+i)) << ": " << buffer << endl;
	}
	return os;
}

std::vector< SpritePalette::Color >& SpritePalette::getColors() {
	return colors;
}

//////////

std::ostream& operator<<(std::ostream&os, const SpriteFrame&p) {
	os << "SpriteFrame: (width=" << (int)p.width << "):\n";
	for(int yT=0;yT<((p.nPixels/64)/p.width);yT++)
		for(int y=0;y<8;y++) {
			for(int xT=0;xT<p.width;xT++)
				for(int x=0;x<8;x++) {
					uint8_t tileID = xT + p.width*yT;
					uint8_t *pD = p.data+x+8*y+64*tileID;
					if(*pD==0xFF)
						os << '.';
					else
						if(*pD > 0xF)
							throw logic_error("Some weird color ID in sprite");
						else if (*pD > 0x9)
							os << (char)(*pD-10+'A');
						else
							os << (char)(*pD+'0');
				}
			os << endl;
		}
	return os;
}

SpriteFrame::SpriteFrame(size_t nPixels) : nPixels(nPixels) {
	data = new uint8_t[nPixels];
	width=4;
}

SpriteFrame::SpriteFrame(const SpriteFrame&other) : nPixels(other.nPixels) {
	data = new uint8_t[nPixels];
	memmove(data,other.data,nPixels);
	width=other.width;
}

SpriteFrame& SpriteFrame::operator=(SpriteFrame other) {
	delete[] data;
	nPixels=other.nPixels;
	data = new uint8_t[nPixels];
	memmove(data,other.data,nPixels);
	width=other.width;
	return *this;
}

SpriteFrame::~SpriteFrame() {
	delete[] data;
}

uint8_t* SpriteFrame::getData() {
	return data;
}
