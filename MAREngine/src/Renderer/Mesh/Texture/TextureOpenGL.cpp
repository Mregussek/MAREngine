/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "TextureOpenGL.h"

namespace mar {
	void TextureOpenGL::shutdown() {
		for (auto const& id : _id)
			glDeleteTextures(1, &id);
	}

	void TextureOpenGL::loadTexture(const std::string& path) {
		auto pathFound = std::find(_paths.begin(), _paths.end(), path);
		if (pathFound != _paths.end()) { // if path found we don't want to load texture again
			auto index = std::distance(_paths.begin(), pathFound);
			_id.push_back(_id[index]); // push index of that found texture
			return;
		}

		// load new texture and push new 
		_paths.push_back(path);
		_id.push_back(_id.size());

		glGenTextures(1, &_id[_id.size() - 1]);
		glBindTexture(GL_TEXTURE_2D, _id[_id.size() - 1]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		_localBuffer = stbi_load(path.c_str(), &_width, &_height, &_bitPerPixel, 0);

		if (_localBuffer) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _localBuffer);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(_localBuffer);
			return;
		}

		std::cerr << "Failed to load texture" << std::endl;
	}

	void TextureOpenGL::bind(const float& shapeId, const unsigned int& texID) const {
		glBindTextureUnit((unsigned int)shapeId, texID);
	}

	void TextureOpenGL::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const unsigned int& TextureOpenGL::getID(int index) const {
		return _id[index];
	}

	const int& TextureOpenGL::getWidth() const {
		return _width;
	}

	const int& TextureOpenGL::getHeight() const {
		return _height;
	}
}