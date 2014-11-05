#include "spritefile.hpp"
#include "spriteio.hpp"
#include <cstdio>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
using namespace std;

std::ostream& operator<<(std::ostream&os,const SpriteFile&p) {
	os << "SpriteFile:\n";
	os << *p.palette << "(SpriteFile)\n";
	os << p.rawFrames.size() << " raw frames:\n";
	for(int i=0;i<p.rawFrames.size();i++)
		os << '[' << i << ']' << p.rawFrames[i] << endl;
	os << "(SpriteFile)\n";
	os << p.animations.size() << " animations:\n";
	SpritePalette* bakPal = SpriteAnimFrame::gPalette;
	SpriteAnimFrame::gPalette = p.palette;
	for(vector< SpriteAnim >::const_iterator it=p.animations.begin();it!=p.animations.end();++it)
		os << *it;
	SpriteAnimFrame::gPalette = bakPal;
	return os;
}

SpriteFile::SpriteFile(std::istream&file) {
	SpriteIO myIO(file);
	palette = new SpritePalette;
	// Load palette
	{
		const SpriteIOPalette& ioPalette = myIO.getHeader().getSubHeader().getFrameBlock().getPalette();
		for(int i=0;i<ioPalette.getColors().size();i++) {
			const SpriteIOColor& ioColor = ioPalette.getColors()[i];
			SpritePalette::Color newColor;
			newColor.r=ioColor.getRed();
			newColor.g=ioColor.getGreen();
			newColor.b=ioColor.getBlue();
			palette->getColors().push_back(newColor);
		}
	}
	// Load raw frames
	{
		const SpriteIOFrameBlock& ioFBlock = myIO.getHeader().getSubHeader().getFrameBlock();
		for(vector< SpriteIORawFrame >::const_iterator it=ioFBlock.getFrameList().begin();it!=ioFBlock.getFrameList().end();++it) {
			SpriteRawFrame newFrame(it->getNumPixels());
			memmove(newFrame.getData(),it->getMap(),it->getNumPixels());
			rawFrames.push_back(newFrame);
		}
	}
	// Load animations
	{
		const SpriteIOInfoBlock& ioInfo = myIO.getHeader().getSubHeader().getInfoBlock();
		for(int mainID=0;mainID<ioInfo.getDbgList().size();mainID++) {
			const SpriteIODBG& ioDbg = ioInfo.getDbgList()[mainID];
			for(int subID=0;subID<ioDbg.getDBIList().size();subID++) {
				const vector< SpriteIODBI >& ioDbi = ioDbg.getDBIList()[subID];
				SpriteAnim newAnim(mainID,subID);
				for(vector< SpriteIODBI >::const_iterator it=ioDbi.begin();it!=ioDbi.end();++it) {
					const SpriteIODBS& ioDbs = ioInfo.getDbsList()[it->getFrameNo()];
					SpriteAnimFrame newFrame(rawFrames[ioDbs.getFrameID()],ioDbs.getFrameID(),it->getTime(),it->getVal0(),it->getVal1(),it->getVal2(),it->getVal3(),ioDbs.getVal0(),ioDbs.getVal1(),ioDbs.getVal2(),ioDbs.getVal3(),ioDbs.getVal4());
					newAnim.getFrames().push_back(newFrame);
				}
				animations.push_back(newAnim);
			}
		}
	}
}

SpriteFile::SpriteFile(const SpriteFile&other) : rawFrames(other.rawFrames),animations(other.animations) {
	palette = new SpritePalette(*other.palette);
}

SpriteFile::~SpriteFile() {
	delete palette;
}

SpriteFile& SpriteFile::operator=(SpriteFile other) {
	rawFrames=other.rawFrames;
	animations=other.animations;
	delete palette;
	palette = new SpritePalette(*other.palette);
	return *this;
}

const SpritePalette& SpriteFile::getPalette() const {
	return *palette;
}

SpritePalette& SpriteFile::getPalette() {
	return *palette;
}

const std::vector< SpriteRawFrame >& SpriteFile::getRawFrames() const {
	return rawFrames;
}

std::vector< SpriteRawFrame >& SpriteFile::getRawFrames() {
	return rawFrames;
}

const SpriteRawFrame& SpriteFile::getRawFrame(int i) const {
	return rawFrames[i];
}

SpriteRawFrame& SpriteFile::getRawFrame(int i) {
	return rawFrames[i];
}

const std::vector< SpriteAnim >& SpriteFile::getAnimations() const {
	return animations;
}

std::vector< SpriteAnim >& SpriteFile::getAnimations() {
	return animations;
}

const SpriteAnim& SpriteFile::getAnimation(int i, int j) const {
	for(vector< SpriteAnim >::const_iterator it=animations.begin();it!=animations.end();++it)
		if(it->isAnim(i,j))
			return *it;
}

SpriteAnim& SpriteFile::getAnimation(int i, int j) {
	for(vector< SpriteAnim >::iterator it=animations.begin();it!=animations.end();++it)
		if(it->isAnim(i,j))
			return *it;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpritePalette&p) {
	os << "SpritePalette (" << p.colors.size() << " entries):\n";
	char buffer[64];
	for(int i=0;i<p.colors.size();i++) {
		sprintf(buffer,"0x%1$1X  R=%2$03u  G=%3$03u  B=%4$03u   #%2$02X%3$02X%4$02X\n",i,p[i].r,p[i].g,p[i].b);
		os << buffer;
	}
	return os;
}

const SpritePalette::Color& SpritePalette::operator[](int i) const {
	return colors[i];
}

SpritePalette::Color& SpritePalette::operator[](int i) {
	return colors[i];
}

const std::vector< SpritePalette::Color >& SpritePalette::getColors() const {
	return colors;
}

std::vector< SpritePalette::Color >& SpritePalette::getColors() {
	return colors;
}

std::ostream& SpritePalette::render(std::ostream&os,uint8_t id) const {
	uint8_t highBits = (((colors[id].r)>>5)&4) | (((colors[id].g)>>6)&2) | (((colors[id].b)>>7)&1);
	const char ansiColors[] = {'0','4','2','6','1','5','3','7'};
	os << "\x1b[4" << ansiColors[highBits] << ";3" << ansiColors[highBits^7] << "m";
	if(id==0)
		os << "..";
	else if(id<10)
		os << (char)('0'+id) << (char)('0'+id);
	else
		os << (char)('A'-10+id) << (char)('A'-10+id);
	os << "\x1b[39;49m";
	return os;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteRawFrame&p) {
	os << "SpriteRawFrame: " << p.nPixels << "px = " << p.nPixels/64 << " tiles";
	return os;
}

SpriteRawFrame::SpriteRawFrame(size_t nPixels) : nPixels(nPixels) {
	pData = new uint8_t[nPixels];
}

SpriteRawFrame::SpriteRawFrame(const SpriteRawFrame&other) : nPixels(other.nPixels) {
	pData = new uint8_t[nPixels];
	memmove(pData,other.pData,nPixels);
}

SpriteRawFrame::~SpriteRawFrame() {
	delete[] pData;
}

SpriteRawFrame& SpriteRawFrame::operator=(SpriteRawFrame other) {
	nPixels=other.nPixels;
	delete[] pData;
	pData = new uint8_t[nPixels];
	memmove(pData,other.pData,nPixels);
	return *this;
}

size_t SpriteRawFrame::getNPixels() const {
	return nPixels;
}

const uint8_t* SpriteRawFrame::getData() const {
	return pData;
}

uint8_t* SpriteRawFrame::getData() {
	return pData;
}

#define PIXEL	pData[x+8*y+64*xT+64*width*yT]
std::ostream& SpriteRawFrame::render(std::ostream&os,int width,const SpritePalette* palette) const {
	os << "   ";
	for(int x=0;x<(8*width);x++) {
		if(palette)
			os << ' ';
		os << x/10;
	}
	os << "\n   ";
	for(int x=0;x<(8*width);x++) {
		if(palette)
			os << ' ';
		os << x%10;
	}
	os << endl;
	for(int yT=0;yT<(nPixels/64/width);yT++)
		for(int y=0;y<8;y++) {
			if((y+8*yT)<10)
				os << ' ';
			os << y+8*yT << ' ';
			for(int xT=0;xT<width;xT++)
				for(int x=0;x<8;x++)
					if(palette)
						palette->render(os,PIXEL);
					else {
						if(PIXEL==0)
							os << '.';
						else if (PIXEL<10)
							os << (char)('0'+PIXEL);
						else
							os << (char)('A'-10+PIXEL);
					}
			os << endl;
		}
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteAnim&p) {
	os << "SpriteAnim: " << p.mainID << ':' << p.subID << endl;
	os << p.frames.size() << " frames:\n";
	for(vector< SpriteAnimFrame >::const_iterator it=p.frames.begin();it!=p.frames.end();++it)
		os << *it;
	return os;
}

SpriteAnim::SpriteAnim(int mainID,int subID) : mainID(mainID),subID(subID) {
}

bool SpriteAnim::isAnim(int i,int j) const {
	return ((mainID==i)&&(subID==j));
}

int SpriteAnim::getMainID() const {
	return mainID;
}

int SpriteAnim::getSubID() const {
	return subID;
}

const std::vector< SpriteAnimFrame >& SpriteAnim::getFrames() const {
	return frames;
}

std::vector< SpriteAnimFrame >& SpriteAnim::getFrames() {
	return frames;
}

const SpriteAnimFrame& SpriteAnim::operator[] (int i) const {
	return frames[i];
}

SpriteAnimFrame& SpriteAnim::operator[] (int i) {
	return frames[i];
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteAnimFrame&p) {
	os << "SpriteAnimFrame: time=" << p.time << ",width=" << p.width << endl;
	char buffer[64];
	sprintf(buffer,"raw=%4i    %4i  dY=%4i  %4i  %4i | yC=%4i  0x%02X  xC=%4i  0x%02X  0x%04X\n",p.rawID,(int16_t)p.val0,(int16_t)p.val1,(int16_t)p.val2,(int16_t)p.val3,p.yC,(int)p.val5,p.xC,(int)p.val7,p.val8);
	os << buffer;
	return p.render(os,p.width,p.gPalette);
}

SpritePalette* SpriteAnimFrame::gPalette = 0;

SpriteAnimFrame::SpriteAnimFrame(const SpriteRawFrame&p,int rawID,int time,uint16_t val0,uint16_t val1,uint16_t val2,uint16_t val3,int8_t yC,uint8_t val5,int8_t xC,uint8_t val7,uint16_t val8) : SpriteRawFrame(p),time(time),width(1),val0(val0),val1(val1),val2(val2),val3(val3),yC(yC^0xFF),val5(val5),xC(xC^0xFF),val7(val7),val8(val8),rawID(rawID) {
	// Figure out the size
	int tileCount = getNPixels()/64;
	switch(val5&0xF0) {
		case 0x0:
			// Square ratio
			for(width=1;width<tileCount;width++)
				if(tileCount==(width*width))
					break;
			break;
		case 0x40:
			// Wide ratio
			for(width=1;width<tileCount;width++)
				if((2*tileCount)==(width*width))
					break;
			break;
		case 0x80:
			// Tall ratio
			for(width=1;width<tileCount;width++)
				if(tileCount=(width*width*2))
					break;
			break;
		default:
			throw logic_error("RESEARCH THIS! (size)");
	}

	// Handle val7 flags
//	if((val7 & 0xE7) != 0x80)
//		throw logic_error("RESEARCH THIS! (val7)");
	if(val7&0x10) { // Do X flip
		uint8_t* xFlipped = new uint8_t[getNPixels()];
		for(int tY=0;tY<getHeight();tY++)
			for(int y=0;y<8;y++)
				for(int tX=0;tX<width;tX++)
					for(int x=0;x<8;x++)
						xFlipped[(7-x)+8*y+(width-tX-1)*64+64*width*tY] = getData()[x+8*y+tX*64+tY*64*width];
		memmove(getData(),xFlipped,getNPixels());
		delete[] xFlipped;
	}

}

int SpriteAnimFrame::getTime() const {
	return time;
}

int SpriteAnimFrame::getWidth() const {
	return width;
}

int SpriteAnimFrame::getHeight() const {
	return getNPixels()/64/width;
}

SpriteAnimFrame& SpriteAnimFrame::setWidth(int width) {
	this->width=width;
	return *this;
}
