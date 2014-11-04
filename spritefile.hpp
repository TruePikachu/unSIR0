#ifndef __SPRITEFILE_HPP
#define __SPRITEFILE_HPP
class SpriteFile;
class SpritePalette;
class SpriteFrame;
#include <istream>
#include <ostream>
#include <stdint.h>
#include <vector>

class SpriteFile {
	friend std::ostream& operator<<(std::ostream&, const SpriteFile&);
	private:
		SpritePalette*			palette;
		std::vector< SpriteFrame >	frames;
	public:
				 SpriteFile	(std::istream&);
				 SpriteFile	(const SpriteFile&);
				~SpriteFile	();
		SpriteFile&	operator=	(SpriteFile);

		const SpritePalette&			getPalette	() const;
		SpritePalette&				getPalette	();
		const std::vector< SpriteFrame >&	getFrames	() const;
		std::vector< SpriteFrame >&		getFrames	();
		const SpriteFrame&			operator[]	(int) const;
		SpriteFrame&				operator[]	(int);
};

class SpritePalette {
	friend std::ostream& operator<<(std::ostream&, const SpritePalette&);
	public:
		typedef struct {
			uint8_t	r;
			uint8_t	g;
			uint8_t	b;
			uint8_t	a;
		} Color;
	private:
		std::vector< Color >	colors;
	public:
		const std::vector< Color >&	getColors	() const;
		std::vector< Color >&		getColors	();
		const Color&			operator[]	(int) const;
		Color&				operator[]	(int);
};

class SpriteFrame {
	friend std::ostream& operator<<(std::ostream&, const SpriteFrame&);
	private:
		size_t		x;
		size_t		y;
		uint8_t *	data;
	public:
				 SpriteFrame	(size_t x, size_t y);
				 SpriteFrame	(const SpriteFrame&);
				~SpriteFrame	();
		SpriteFrame&	operator=	(SpriteFrame);
};

#endif
