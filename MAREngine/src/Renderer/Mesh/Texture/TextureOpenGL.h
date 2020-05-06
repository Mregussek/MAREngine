/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_OPENGL_H
#define TEXTURE_OPENGL_H

#include "Texture.h"

namespace mar {
	class TextureOpenGL : public Texture, std::enable_shared_from_this<TextureOpenGL> {
		std::vector<unsigned int> _id;
		std::vector<std::string> _paths;
		unsigned char* _localBuffer;
		int _width;
		int _height;
		int _bitPerPixel;

	public:
		TextureOpenGL() = default;
		~TextureOpenGL() override;

		void loadTexture(const std::string& path) override;

		void bind(const float& shapeId, const unsigned int& texID) const override;
		void unbind() const override;

		const unsigned int& getID(int index) const override { return _id[index]; }
		const int& getWidth() const override { return _width; }
		const int& getHeight() const override { return _height; }
	};
}

#endif // !TEXTURE_OPENGL_H