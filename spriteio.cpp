#include "spriteio.hpp"
#include <cstdio>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
using namespace std;

std::ostream& operator<<(std::ostream&os,const SpriteIO&p) {
	os << "SpriteIO: sData=" << p.sData << endl;
	os << *p.header << "(SpriteIO)\n";
	return os;
}

SpriteIO::SpriteIO(std::istream&file) {
	file.seekg(0,file.end);
	sData=file.tellg();
	file.seekg(0);
	header = new SpriteIOHeader(file);
}

SpriteIO::SpriteIO(const SpriteIO&other) : sData(other.sData) {
	header = new SpriteIOHeader(*other.header);
}

SpriteIO::~SpriteIO() {
	delete header;
}

SpriteIO& SpriteIO::operator=(SpriteIO other) {
	delete header;
	header = new SpriteIOHeader(*other.header);
	sData=other.sData;
	return *this;
}

size_t SpriteIO::getDataSize() const {
	return sData;
}

const SpriteIOHeader& SpriteIO::getHeader() const {
	return *header;
}

SpriteIOHeader& SpriteIO::getHeader() {
	return *header;
}

//////////

#define STRUCT_START	off_t myStart = file.tellg()
#define STRUCT_SEEK(x)	file.seekg(myStart+x)
#define FILE_SEEK(x)	file.seekg(x)
#define NEW_DWORD(x)	uint32_t x; file.read((char*)&x,4)
#define NEW_WORD(x)	uint16_t x; file.read((char*)&x,2)
#define NEW_BYTE(x)	uint8_t x; file.read((char*)&x,1)
#define READ_DWORD(x)	file.read((char*)&x,4)
#define READ_WORD(x)	file.read((char*)&x,2)
#define READ_BYTE(x)	file.read((char*)&x,1)
#define READ(x)		file.read((char*)&x,sizeof(x))

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOHeader&p) {
	os << "SpriteIOHeader: off_b=" << p.off_b << endl;
	os << *p.subHeader << "(SpriteIOHeader)\n";
	return os;
}

SpriteIOHeader::SpriteIOHeader(std::istream&file) {
	STRUCT_START;
	NEW_DWORD(magic);
	if(magic!=0x30524953)
		throw runtime_error("Not a SIR0 file");
	NEW_DWORD(offSubHeader);
	READ_DWORD(off_b);
	NEW_DWORD(null);
	if(null)
		throw runtime_error("SpriteIOHeader+0xC != 0");
	FILE_SEEK(offSubHeader);
	subHeader = new SpriteIOSubHeader(file);
}

SpriteIOHeader::SpriteIOHeader(const SpriteIOHeader& other) : off_b(other.off_b) {
	subHeader = new SpriteIOSubHeader(*other.subHeader);
}

SpriteIOHeader::~SpriteIOHeader() {
	delete subHeader;
}

SpriteIOHeader& SpriteIOHeader::operator=(SpriteIOHeader other) {
	delete subHeader;
	subHeader = new SpriteIOSubHeader(*other.subHeader);
	off_b=other.off_b;
	return *this;
}

const SpriteIOSubHeader& SpriteIOHeader::getSubHeader() const {
	return *subHeader;
}

SpriteIOSubHeader& SpriteIOHeader::getSubHeader() {
	return *subHeader;
}

uint32_t SpriteIOHeader::getOffB() const {
	return off_b;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOSubHeader&p) {
	os << "SpriteIOSubHeader: unkBool1=" << p.unkBool1 << ",unkBool2=" << p.unkBool2 << endl;
	os << *p.infoBlock << "(SpriteIOSubHeader)\n";
	os << *p.frameBlock << "(SpriteIOSubHeader)\n";
	return os;
}

SpriteIOSubHeader::SpriteIOSubHeader(std::istream&file) {
	STRUCT_START;
	NEW_DWORD(offInfoBlock);
	NEW_DWORD(offFrameBlock);
	READ_WORD(unkBool1);
	READ_WORD(unkBool2);
	FILE_SEEK(offInfoBlock);
	infoBlock = new SpriteIOInfoBlock(file);
	FILE_SEEK(offFrameBlock);
	frameBlock = new SpriteIOFrameBlock(file);
}

SpriteIOSubHeader::SpriteIOSubHeader(const SpriteIOSubHeader&other) : unkBool1(other.unkBool1),unkBool2(other.unkBool2) {
	infoBlock = new SpriteIOInfoBlock(*other.infoBlock);
	frameBlock = new SpriteIOFrameBlock(*other.frameBlock);
}

SpriteIOSubHeader::~SpriteIOSubHeader() {
	delete infoBlock;
	delete frameBlock;
}

SpriteIOSubHeader& SpriteIOSubHeader::operator=(SpriteIOSubHeader other) {
	unkBool1=other.unkBool1;
	unkBool2=other.unkBool2;
	delete infoBlock;
	delete frameBlock;
	infoBlock = new SpriteIOInfoBlock(*other.infoBlock);
	frameBlock = new SpriteIOFrameBlock(*other.frameBlock);
	return *this;
}

const SpriteIOInfoBlock& SpriteIOSubHeader::getInfoBlock() const {
	return *infoBlock;
}

SpriteIOInfoBlock& SpriteIOSubHeader::getInfoBlock() {
	return *infoBlock;
}

const SpriteIOFrameBlock& SpriteIOSubHeader::getFrameBlock() const {
	return *frameBlock;
}

SpriteIOFrameBlock& SpriteIOSubHeader::getFrameBlock() {
	return *frameBlock;
}

uint16_t SpriteIOSubHeader::getUnkBool1() const {
	return unkBool1;
}

uint16_t SpriteIOSubHeader::getUnkBool2() const {
	return unkBool2;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOInfoBlock&p) {
	os << "SpriteIOInfoBlock: unkInt1=" << p.unkInt1 << ",unkBool3=" << p.unkBool3 << ",unkBool4=" << p.unkBool4 << ",unkBool5=" << p.unkBool5 << ",unkBool6=" << p.unkBool6 << endl;
	os << "dbsList (" << p.dbsList.size() << " entries):\n";
	for(int i=0;i<p.dbsList.size();i++)
		os << "<dbsList#" << i << ">\n" << p.dbsList[i] << endl;
	os << "(SpriteIOInfoBlock)\n";
	os << "fpairList (" << p.fpairList.size() << " entries):\n";
	for(int i=0;i<p.fpairList.size();i++)
		os << "<fpairList#" << i << ">\n" << p.fpairList[i] << endl;
	os << "(SpriteIOInfoBlock)\n";
	os << "dbgList (" << p.dbgList.size() << " entries):\n";
	for(int i=0;i<p.dbgList.size();i++)
		os << "<dbgList#" << i << ">\n" << p.dbgList[i] << endl;
	os << "(SpriteIOInfoBlock)\n";
	return os;
}

SpriteIOInfoBlock::SpriteIOInfoBlock(std::istream&file) {
	STRUCT_START;
	NEW_DWORD(offDbsList);
	NEW_DWORD(offFpairList);
	NEW_DWORD(offDbgList);
	NEW_WORD(nbEntriesOffG);
	READ_WORD(unkInt1);
	READ_WORD(unkBool3);
	READ_WORD(unkBool4);
	READ_WORD(unkBool5);
	READ_WORD(unkBool6);
	for(int i=0;i<((offFpairList-offDbsList)/4);i++) {
		FILE_SEEK(offDbsList+4*i);
		NEW_DWORD(offNewDbs);
		FILE_SEEK(offNewDbs);
		dbsList.push_back(SpriteIODBS(file));
	}
	// In theory, the FpairList is ended by the first DBG address
	// FIXME Problems happen if e.g. offFirstDbg==0 (m_attack.bin:0x0000)
	// FIXME It is not known how this can happen, unless the file isn't actually good
	FILE_SEEK(offDbgList);
	NEW_DWORD(offFirstDbg);
	if(!offFirstDbg)
		throw logic_error("FIXME offFirstDbg==0?!");
	for(int i=0;i<((offFirstDbg-offFpairList)/4);i++) {
		FILE_SEEK(offFpairList+4*i);
		fpairList.push_back(SpriteIOFPair(file));
	}
	for(int i=0;i<nbEntriesOffG;i++) {
		FILE_SEEK(offDbgList+8*i);
		dbgList.push_back(SpriteIODBG(file));
	}
}

const std::vector< SpriteIODBS >& SpriteIOInfoBlock::getDbsList() const {
	return dbsList;
}

std::vector< SpriteIODBS >& SpriteIOInfoBlock::getDbsList() {
	return dbsList;
}

const std::vector< SpriteIOFPair >& SpriteIOInfoBlock::getFPairList() const {
	return fpairList;
}

std::vector< SpriteIOFPair >& SpriteIOInfoBlock::getFPairList() {
	return fpairList;
}

const std::vector< SpriteIODBG >& SpriteIOInfoBlock::getDbgList() const {
	return dbgList;
}

std::vector< SpriteIODBG >& SpriteIOInfoBlock::getDbgList() {
	return dbgList;
}

uint16_t SpriteIOInfoBlock::getUnkInt1() const {
	return unkInt1;
}

uint16_t SpriteIOInfoBlock::getUnkBool3() const {
	return unkBool3;
}

uint16_t SpriteIOInfoBlock::getUnkBool4() const {
	return unkBool4;
}

uint16_t SpriteIOInfoBlock::getUnkBool5() const {
	return unkBool5;
}

uint16_t SpriteIOInfoBlock::getUnkBool6() const {
	return unkBool6;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOFrameBlock&p) {
	os << "SpriteIOFrameBlock: unkBool7=" << p.unkBool7 << ",unkBool8=" << p.unkBool8 << ",unkInt=" << p.unkInt << endl;
	os << "frameList (" << p.frameList.size() << " entries):\n";
	for(int i=0;i<p.frameList.size();i++)
		os << "<frameList#" << i << ">\n" << p.frameList[i] << endl;
	os << "(SpriteIOFrameBlock)\n";
	os << *p.palette << "SpriteIOFrameBlock)\n";
	return os;
}

SpriteIOFrameBlock::SpriteIOFrameBlock(std::istream&file) {
	STRUCT_START;
	NEW_DWORD(offFrameTable);
	NEW_DWORD(offPalette);
	READ_WORD(unkBool7);
	READ_WORD(unkBool8);
	READ_WORD(unkInt);
	NEW_WORD(nbFrames);
	for(int i=0;i<nbFrames;i++) {
		FILE_SEEK(offFrameTable+4*i);
		NEW_DWORD(offNewFrame);
		FILE_SEEK(offNewFrame);
		frameList.push_back(SpriteIORawFrame(file));
	}
	FILE_SEEK(offPalette);
	palette = new SpriteIOPalette(file);
}

SpriteIOFrameBlock::SpriteIOFrameBlock(const SpriteIOFrameBlock&other) : unkBool7(other.unkBool7),unkBool8(other.unkBool8),unkInt(other.unkInt) {
	frameList=other.frameList;
	palette = new SpriteIOPalette(*other.palette);
}

SpriteIOFrameBlock::~SpriteIOFrameBlock() {
	delete palette;
}

SpriteIOFrameBlock& SpriteIOFrameBlock::operator=(SpriteIOFrameBlock other) {
	unkBool7=other.unkBool7;
	unkBool8=other.unkBool8;
	unkInt=other.unkInt;
	frameList=other.frameList;
	palette = new SpriteIOPalette(*other.palette);
	return *this;
}

const std::vector< SpriteIORawFrame >& SpriteIOFrameBlock::getFrameList() const {
	return frameList;
}

std::vector< SpriteIORawFrame >& SpriteIOFrameBlock::getFrameList() {
	return frameList;
}

const SpriteIOPalette& SpriteIOFrameBlock::getPalette() const {
	return *palette;
}

SpriteIOPalette& SpriteIOFrameBlock::getPalette() {
	return *palette;
}

uint16_t SpriteIOFrameBlock::getUnkBool7() const {
	return unkBool7;
}

uint16_t SpriteIOFrameBlock::getUnkBool8() const {
	return unkBool8;
}

uint16_t SpriteIOFrameBlock::getUnkInt() const {
	return unkInt;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOPalette&p) {
	os << "SpriteIOPalette: unknown0=" << p.unknown0 << ",unknown1=" << p.unknown1 << ",unknown2=" << p.unknown2 << ",unknown3=" << p.unknown3 << ",endOfPalData=" << p.endOfPalData << endl;
	os << "colors (" << p.colors.size() << " entries):\n";
	for(int i=0;i<p.colors.size();i++)
		os << "<colors#" << i << ">\n" << p.colors[i] << endl;
	os << "(SpriteIOPalette)\n";
	return os;
}

SpriteIOPalette::SpriteIOPalette(std::istream&file) {
	STRUCT_START;
	NEW_DWORD(offPaletteData);
	READ_WORD(unknown0);
	READ_WORD(unknown1);
	READ_WORD(unknown2);
	READ_WORD(unknown3);
	READ_DWORD(endOfPalData);
	for(int i=0;i<16;i++) {
		FILE_SEEK(offPaletteData+4*i);
		colors.push_back(SpriteIOColor(file));
	}
}

const std::vector< SpriteIOColor >& SpriteIOPalette::getColors() const {
	return colors;
}

std::vector< SpriteIOColor >& SpriteIOPalette::getColors() {
	return colors;
}

uint16_t SpriteIOPalette::getUnk0() const {
	return unknown0;
}

uint16_t SpriteIOPalette::getUnk1() const {
	return unknown1;
}

uint16_t SpriteIOPalette::getUnk2() const {
	return unknown2;
}

uint16_t SpriteIOPalette::getUnk3() const {
	return unknown3;
}

uint32_t SpriteIOPalette::getUnk4() const {
	return endOfPalData;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOColor&p) {
	os << "SpriteIOColor: ";
	char buf[64];
	sprintf(buf,"#%02X%02X%02X",p.r,p.g,p.b);
	os << buf << endl;
	return os;
}

SpriteIOColor::SpriteIOColor(std::istream&file) {
	STRUCT_START;
	READ_BYTE(r);
	READ_BYTE(g);
	READ_BYTE(b);
	NEW_BYTE(x);
	if(x!=0x80)
		throw runtime_error("Weird color?");
}

uint8_t SpriteIOColor::getRed() const {
	return r;
}

uint8_t SpriteIOColor::getGreen() const {
	return g;
}

uint8_t SpriteIOColor::getBlue() const {
	return b;
}

//////////
std::ostream& operator<<(std::ostream&os,const SpriteIORawFrame&p) {
	os << "SpriteIORawFrame: " << p.numPixels << "px = " << p.numPixels/64 << endl;
	for(int width=1;width<=(p.numPixels/64);width++)
		if(!((p.numPixels/64)%width)) {
			os << width << 'x' << p.numPixels/64/width << ":\n";
			p.renderWithWidth(os,width);
			os << endl;
		}
	return os;
}

typedef struct{uint32_t source;uint32_t size;uint32_t unknown;}FrameCommand;
SpriteIORawFrame::SpriteIORawFrame(std::istream&file) {
	STRUCT_START;
	vector< FrameCommand > commands;
	numPixels=0;
	for(;;) {
		FrameCommand newCommand;
		READ(newCommand);
		if(newCommand.size)
			commands.push_back(newCommand);
		else
			break;
		numPixels += newCommand.size*2;
	}
	colorMap = new uint8_t[numPixels];
	uint8_t* writePos=colorMap;
	for(vector< FrameCommand >::iterator it=commands.begin();it!=commands.end();++it) {
		if(it->source) {
			FILE_SEEK(it->source);
			for(int i=0;i<it->size;i++) {
				NEW_BYTE(twoPx);
				*(writePos++) = (twoPx >> 0) & 0xF;
				*(writePos++) = (twoPx >> 4) & 0xF;
			}
		} else {
			memset(writePos,0,it->size*2);
			writePos += it->size*2;
		}
	}
}

SpriteIORawFrame::SpriteIORawFrame(const SpriteIORawFrame&other) : numPixels(other.numPixels) {
	colorMap = new uint8_t[numPixels];
	memmove(colorMap,other.colorMap,numPixels);
}

SpriteIORawFrame::~SpriteIORawFrame() {
	delete[] colorMap;
}

SpriteIORawFrame& SpriteIORawFrame::operator=(SpriteIORawFrame other) {
	delete[] colorMap;
	numPixels=other.numPixels;
	colorMap = new uint8_t[numPixels];
	memmove(colorMap,other.colorMap,numPixels);
	return *this;
}

size_t SpriteIORawFrame::getNumPixels() const {
	return numPixels;
}

const uint8_t* SpriteIORawFrame::getMap() const {
	return colorMap;
}

#define PIXEL colorMap[x+8*y+64*tX+64*width*tY]
std::ostream& SpriteIORawFrame::renderWithWidth(std::ostream&os,int width) const {
	int height = numPixels/64/width;
	for(int tY=0;tY<height;tY++)
		for(int y=0;y<8;y++) {
			for(int tX=0;tX<width;tX++)
				for(int x=0;x<8;x++)
					if(PIXEL==0)
						os << '.';
					else if (PIXEL<10)
						os << (char)('0'+PIXEL);
					else
						os << (char)('A'-10+PIXEL);
			os << endl;
		}
	return os << endl;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIODBS&p) {
	os << "SpriteIODBS: frameID=" << p.frameID << ",val0=" << (int)p.val0 << ",val1=" << (int)p.val1 << ",val2=" << (int)p.val2 << ",val3=" << (int)p.val3 << ",val4=" << p.val4 << endl;
	return os;
}

SpriteIODBS::SpriteIODBS(std::istream&file) {
	STRUCT_START;
	READ_DWORD(frameID);
	READ_BYTE(val0);
	READ_BYTE(val1);
	READ_BYTE(val2);
	READ_BYTE(val3);
	READ_WORD(val4);
}

uint32_t SpriteIODBS::getFrameID() const {
	return frameID;
}

uint8_t SpriteIODBS::getVal0() const {
	return val0;
}

uint8_t SpriteIODBS::getVal1() const {
	return val1;
}

uint8_t SpriteIODBS::getVal2() const {
	return val2;
}

uint8_t SpriteIODBS::getVal3() const {
	return val3;
}

uint16_t SpriteIODBS::getVal4() const {
	return val4;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIOFPair&p) {
	os << "SpriteIOFPair: " << p.n1 << ',' << p.n2 << endl;
	return os;
}

SpriteIOFPair::SpriteIOFPair(std::istream&file) {
	STRUCT_START;
	READ_WORD(n1);
	READ_WORD(n2);
}

int16_t SpriteIOFPair::getN1() const {
	return n1;
}

int16_t SpriteIOFPair::getN2() const {
	return n2;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIODBG&p) {
	os << "SpriteIODBG:\n";
	os << "dbiList (" << p.dbiList.size() << " subanim):\n";
	for(int i=0;i<p.dbiList.size();i++) {
		os << "<dbiList#" << i << "> n=" << p.dbiList[i].size() << endl;
		for(int j=0;j<p.dbiList[i].size();j++)
			os << "<dbiList#" << i << ':' << j << "> " << p.dbiList[i][j];
	}
	os << "(SpriteIODBG)\n";
	return os;
}

SpriteIODBG::SpriteIODBG(std::istream&file) {
	STRUCT_START;
	NEW_DWORD(offTable);
	NEW_DWORD(nbEntries);
	for(int i=0;i<nbEntries;i++) {
		FILE_SEEK(offTable+4*i);
		NEW_DWORD(offDBI);
		FILE_SEEK(offDBI);
		// At the first entry in a list of DBIs
		vector< SpriteIODBI > subAnim;
		for(;;) {
			SpriteIODBI newDbi(file);
			if(newDbi.getTime())
				subAnim.push_back(newDbi);
			else
				break;
		}
		dbiList.push_back(subAnim);
	}
}

const std::vector< std::vector< SpriteIODBI > >& SpriteIODBG::getDBIList() const {
	return dbiList;
}

std::vector< std::vector< SpriteIODBI > >& SpriteIODBG::getDBIList() {
	return dbiList;
}

//////////

std::ostream& operator<<(std::ostream&os,const SpriteIODBI&p) {
	os << "SpriteIODBI: time=" << p.time << ",dbsFrame=" << p.dbsFrame << ",val0=" << p.val0 << ",val1=" << p.val1 << ",val2=" << p.val2 << ",val3=" << p.val3 << endl;
	return os;
}

SpriteIODBI::SpriteIODBI(std::istream&file) {
	STRUCT_START;
	READ_WORD(time);
	READ_WORD(dbsFrame);
	READ_WORD(val0);
	READ_WORD(val1);
	READ_WORD(val2);
	READ_WORD(val3);
}

uint16_t SpriteIODBI::getTime() const {
	return time;
}

uint16_t SpriteIODBI::getFrameNo() const {
	return dbsFrame;
}

uint16_t SpriteIODBI::getVal0() const {
	return val0;
}

uint16_t SpriteIODBI::getVal1() const {
	return val1;
}

uint16_t SpriteIODBI::getVal2() const {
	return val2;
}

uint16_t SpriteIODBI::getVal3() const {
	return val3;
}
