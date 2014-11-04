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
	FILE_SEEK(offDbgList);
	NEW_DWORD(offFirstDbg);
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
