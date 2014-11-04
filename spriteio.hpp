#ifndef __SPRITEIO_HPP
#define __SPRITEIO_HPP
class SpriteIO;
class SpriteIOHeader;
class SpriteIOSubHeader;
class SpriteIOInfoBlock;
class SpriteIOFrameBlock;
class SpriteIOPalette;
class SpriteIOColor;
class SpriteIORawFrame;
class SpriteIODBS;
class SpriteIOFPair;
class SpriteIODBG;
class SpriteIODBI;
#include <istream>
#include <ostream>
#include <stdint.h>
#include <vector>

class SpriteIO {
	friend std::ostream& operator<<(std::ostream&,const SpriteIO&);
	private:
		size_t		sData;
		char*		fData;
		SpriteIOHeader*	header;
	public:
					 SpriteIO	(std::istream&);
					 SpriteIO	(const SpriteIO&);
					~SpriteIO	();
		SpriteIO&		operator=	(SpriteIO);
		size_t			getDataSize	() const;
		const char*		getData		() const;
		const SpriteIOHeader&	getHeader	() const;
		SpriteIOHeader&		getHeader	();
};

class SpriteIOHeader {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOHeader&);
	private:
		SpriteIOSubHeader*	subHeader;
		uint32_t		off_b;
	public:
				 SpriteIOHeader		(std::istream&);
				 SpriteIOHeader		(const SpriteIOHeader&);
				~SpriteIOHeader		();
		SpriteIOHeader&	operator=		(SpriteIOHeader);
		const SpriteIOSubHeader& getSubHeader	() const;
		SpriteIOSubHeader& getSubHeader		();
		uint32_t	getOffB			() const;
};

class SpriteIOSubHeader {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOSubHeader&);
	private:
		SpriteIOInfoBlock*	infoBlock;
		SpriteIOFrameBlock*	frameBlock;
		uint16_t		unkBool1;
		uint16_t		unkBool2;

	public:
					 SpriteIOSubHeader	(std::istream&);
					 SpriteIOSubHeader	(const SpriteIOSubHeader&);
					~SpriteIOSubHeader	();
		SpriteIOSubHeader&	operator=		(SpriteIOSubHeader);
		const SpriteIOInfoBlock& getInfoBlock		() const;
		SpriteIOInfoBlock&	getInfoBlock		();
		const SpriteIOFrameBlock& getFrameBlock		() const;
		SpriteIOFrameBlock&	getFrameBlock		();
		uint16_t		getUnkBool1		() const;
		uint16_t		getUnkBool2		() const;
};

class SpriteIOInfoBlock {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOInfoBlock&);
	private:
		std::vector< SpriteIODBS >	dbsList;
		std::vector< SpriteIOFPair >	fpairList;
		std::vector< SpriteIODBG >	dbgList;
		uint16_t			unkInt1;
		uint16_t			unkBool3;
		uint16_t			unkBool4;
		uint16_t			unkBool5;
		uint16_t			unkBool6;
	public:
					 SpriteIOInfoBlock	(std::istream&);
					 SpriteIOInfoBlock	(const SpriteIOInfoBlock&);
					~SpriteIOInfoBlock	();
		SpriteIOInfoBlock&	operator=		(SpriteIOInfoBlock);
		const std::vector< SpriteIODBS >& getDbsList	() const;
		std::vector< SpriteIODBS >& getDbsList		();
		const std::vector< SpriteIOFPair >& getFPairList() const;
		std::vector< SpriteIOFPair >& getFPairList	();
		const std::vector< SpriteIODBG >& getDbgList	() const;
		std::vector< SpriteIODBG >& getDbgList		();
		uint16_t		getUnkInt1		() const;
		uint16_t		getUnkBool3		() const;
		uint16_t		getUnkBool4		() const;
		uint16_t		getUnkBool5		() const;
		uint16_t		getUnkBool6		() const;
};

class SpriteIOFrameBlock {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOFrameBlock&);
	private:
		std::vector< SpriteIORawFrame >	frameList;
		SpriteIOPalette*		palette;
		uint16_t			unkBool7;
		uint16_t			unkBool8;
		uint16_t			unkInt;
	public:
					 SpriteIOFrameBlock	(std::istream&);
					 SpriteIOFrameBlock	(const SpriteIOFrameBlock&);
					~SpriteIOFrameBlock	();
		SpriteIOFrameBlock&	operator=		(SpriteIOFrameBlock);
		const std::vector< SpriteIORawFrame >& getFrameList() const;
		std::vector< SpriteIORawFrame >& getFrameList	();
		const SpriteIOPalette&	getPalette		() const;
		SpriteIOPalette&	getPalette		();
		uint16_t		getUnkBool7		() const;
		uint16_t		getUnkBool8		() const;
		uint16_t		getUnkInt		() const;
};

class SpriteIOPalette {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOPalette&);
	private:
		std::vector< SpriteIOColor >	colors;
		uint16_t			unknown0;
		uint16_t			unknown1;
		uint16_t			unknown2;
		uint16_t			unknown3;
		uint32_t			endOfPalData;
	public:
					 SpriteIOPalette	(std::istream&);
		const std::vector< SpriteIOColor >& getColors	() const;
		std::vector< SpriteIOColor >& getColors		();
		uint16_t		getUnk0			() const;
		uint16_t		getUnk1			() const;
		uint16_t		getUnk2			() const;
		uint16_t		getUnk3			() const;
		uint32_t		getUnk4			() const;
};

class SpriteIOColor {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOColor&);
	private:
		uint8_t	r;
		uint8_t	g;
		uint8_t	b;
	public:
				 SpriteIOColor	(std::istream&);
		uint8_t		getRed		() const;
		uint8_t		getGreen	() const;
		uint8_t		getBlue		() const;
};

class SpriteIORawFrame {
	friend std::ostream& operator<<(std::ostream&,const SpriteIORawFrame&);
	private:
		uint8_t*	colorMap;
		size_t		numPixels;
	public:
				 SpriteIORawFrame	(std::istream&);
				 SpriteIORawFrame	(const SpriteIORawFrame&);
				~SpriteIORawFrame	();
		SpriteIORawFrame& operator=		(SpriteIORawFrame);
		size_t		getNumPixels		() const;
		const uint8_t	getMap			() const;
};

class SpriteIODBS {
	friend std::ostream& operator<<(std::ostream&,const SpriteIODBS&);
	private:
		uint32_t	frameID;
		uint8_t		val0;
		uint8_t		val1;
		uint8_t		val2;
		uint8_t		val3;
		uint16_t	val4;
	public:
				 SpriteIODBS	(std::istream&);
		uint32_t	getFrameID	() const;
		uint8_t		getVal0		() const;
		uint8_t		getVal1		() const;
		uint8_t		getVal2		() const;
		uint8_t		getVal3		() const;
		uint16_t	getVal4		() const;
};

class SpriteIOFPair {
	friend std::ostream& operator<<(std::ostream&,const SpriteIOFPair&);
	private:
		int16_t		n1;
		int16_t		n2;
	public:
				 SpriteIOFPair	(std::istream&);
		int16_t		getN1		() const;
		int16_t		getN2		() const;
};

class SpriteIODBG {
	friend std::ostream& operator<<(std::ostream&,const SpriteIODBG&);
	private:
		std::vector< SpriteIODBI >&	dbiList;
	public:
				 SpriteIODBG	(std::istream&);
		const std::vector< SpriteIODBI >& getDBIList() const;
		std::vector< SpriteIODBI >& getDBIList();
};

class SpriteIODBI {
	friend std::ostream& operator<<(std::ostream&,const SpriteIODBI&);
	private:
		uint16_t	time;
		uint16_t	dbsFrame;
		uint16_t	val0;
		uint16_t	val1;
		uint16_t	val2;
		uint16_t	val3;
	public:
				 SpriteIODBI	(std::istream&);
		uint16_t	getTime		() const;
		uint16_t	getFrameNo	() const;
		uint16_t	getVal0		() const;
		uint16_t	getVal1		() const;
		uint16_t	getVal2		() const;
		uint16_t	getVal3		() const;
};

#endif
