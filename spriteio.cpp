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

