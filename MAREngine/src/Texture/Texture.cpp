#include "Texture.h"

Texture::Texture(const std::string& path, size_t how_many)
	: _rendererId(0),
	_filePath(path),
	_localBuffer(nullptr),
	_width(0),
	_height(0),
	_bitPerPixel(0),
	_howMany(how_many)
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(_howMany, &_rendererId);
	glBindTexture(GL_TEXTURE_2D, _rendererId);

	// Black texture unless we don't specify these parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	_localBuffer = stbi_load(_filePath.c_str(), &_width, &_height, &_bitPerPixel, 0);

	if (_localBuffer) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _localBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(_localBuffer);
		return;
	}

	std::cout << "Failed to load texture" << std::endl;
}

Texture::~Texture() {
	glDeleteTextures(_howMany, &_rendererId);
}

void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _rendererId);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}