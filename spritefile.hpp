#ifndef __SPRITEFILE_HPP
#define __SPRITEFILE_HPP
class SpriteFile;
class SpritePalette;
class SpriteRawFrame;
class SpriteAnim;
class SpriteAnimFrame;
#include <istream>
#include <ostream>
#include <stdint.h>
#include <vector>

class SpriteFile {
	friend std::ostream& operator<<(std::ostream&,const SpriteFile&);
	private:
		SpritePalette*			palette;
		std::vector< SpriteRawFrame >	rawFrames;
		std::vector< SpriteAnim >	animations;
	public:
				 SpriteFile	(std::istream&);
				 SpriteFile	(const SpriteFile&);
				~SpriteFile	();
		SpriteFile&	operator=	(SpriteFile);

		const SpritePalette&			getPalette	() const;
		SpritePalette&				getPalette	();
		const std::vector< SpriteRawFrame >&	getRawFrames	() const;
		std::vector< SpriteRawFrame >&		getRawFrames	();
		const SpriteRawFrame&			getRawFrame	(int) const;
		SpriteRawFrame&				getRawFrame	(int);
		const std::vector< SpriteAnim >&	getAnimations	() const;
		std::vector< SpriteAnim >&		getAnimations	();
		const SpriteAnim&			getAnimation	(int,int) const;
		SpriteAnim&				getAnimation	(int,int);
};

class SpritePalette {
	friend std::ostream& operator<<(std::ostream&,const SpritePalette&);
	public:
		typedef struct {uint8_t r;uint8_t g;uint8_t b;} Color;
	private:
		std::vector< Color >		colors;
	public:
		const Color&		operator[]	(int) const;
		Color&			operator[]	(int);
		const std::vector< Color >& getColors	() const;
		std::vector< Color >&	getColors	();
		std::ostream&	render	(std::ostream&,uint8_t) const;
};

class SpriteRawFrame {
	friend std::ostream& operator<<(std::ostream&,const SpriteRawFrame&);
	private:
		size_t		nPixels;
		uint8_t*	pData;
	public:
				 SpriteRawFrame	(size_t);
				 SpriteRawFrame	(const SpriteRawFrame&);
				~SpriteRawFrame	();
		SpriteRawFrame&	operator=	(SpriteRawFrame);

		size_t		getNPixels	() const;
		const uint8_t*	getData		() const;
		uint8_t*	getData		();
		std::ostream&	render		(std::ostream&,int,const SpritePalette* =0) const;
};

class SpriteAnim {
	friend std::ostream& operator<<(std::ostream&,const SpriteAnim&);
	private:
		int				mainID;
		int				subID;
		std::vector< SpriteAnimFrame >	frames;
	public:
					 SpriteAnim	(int,int);
		bool			isAnim		(int,int) const;
		int			getMainID	() const;
		int			getSubID	() const;
		const std::vector< SpriteAnimFrame >& getFrames() const;
		std::vector< SpriteAnimFrame >& getFrames();
		const SpriteAnimFrame&	operator[]	(int) const;
		SpriteAnimFrame&	operator[]	(int);
};

class SpriteAnimFrame : public SpriteRawFrame {
	friend class SpriteFile;
	friend std::ostream& operator<<(std::ostream&,const SpriteAnimFrame&);
	private:
		int			width;
		int			time;
		int			rawID;
		uint16_t	val0;
		uint16_t	val1;
		uint16_t	val2;
		uint16_t	val3;
		uint8_t		val4;
		uint8_t		val5;
		uint8_t		val6;
		uint8_t		val7;
		uint16_t	val8;

	public:
		static SpritePalette*		gPalette;
				 SpriteAnimFrame	(const SpriteRawFrame&,int,int,uint16_t,uint16_t,uint16_t,uint16_t,uint8_t,uint8_t,uint8_t,uint8_t,uint16_t);
		int		getTime			() const;
		int		getWidth		() const;
		int		getHeight		() const;
		SpriteAnimFrame&setWidth		(int);
};

#endif				
