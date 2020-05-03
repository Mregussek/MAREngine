/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../mar.h"

namespace mar {
	class Texture {
		std::vector<unsigned int> _id;
		unsigned char* _localBuffer;
		int _width;
		int _height;
		int _bitPerPixel;

	public:
		Texture();
		~Texture();

		void loadTexture(const std::string& path);

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		const int& getWidth() const { return _width; }
		const int& getHeight() const { return _height; }
	};
}

#endif // !TEXTURE_H