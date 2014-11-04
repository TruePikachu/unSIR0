#include "spriteio.hpp"
#include <cstdio>
#include <cstring>
#include <istream>
#include <ostream>
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
