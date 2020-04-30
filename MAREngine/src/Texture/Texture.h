
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../mar.h"

namespace mar {
	class Texture {
		unsigned int _rendererId;
		std::string _filePath;
		unsigned char* _localBuffer;
		int _width;
		int _height;
		int _bitPerPixel;
		size_t _howMany;

	public:
		Texture(const std::string& path, size_t how_many = 1);
		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		const int& getWidth() const { return _width; }
		const int& getHeight() const { return _height; }
	};
}

#endif // !TEXTURE_H