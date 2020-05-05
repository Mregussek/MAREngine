/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../mar.h"

namespace mar {
	class Texture : std::enable_shared_from_this<Texture> {
		std::vector<unsigned int> _id;
		std::vector<std::string> _paths;
		unsigned char* _localBuffer;
		int _width;
		int _height;
		int _bitPerPixel;

	public:
		Texture() = default;
		Texture(unsigned int slot);
		~Texture();

		void loadTexture(const std::string& path);

		//void bind(unsigned int slot) const;
		void bind(const float& shapeId, const unsigned int& texID) const;
		void unbind() const;

		const unsigned int& getID(int index) const { return _id[index]; }
		const int& getWidth() const { return _width; }
		const int& getHeight() const { return _height; }
	};
}

#endif // !TEXTURE_H