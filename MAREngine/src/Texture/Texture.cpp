#include "Texture.h"

Texture::Texture(const std::string& path, size_t how_many)
	: _filePath(path),
	_localBuffer(nullptr),
	_width(0),
	_height(0),
	_bitPerPixel(0),
	_howMany(how_many)
{
	stbi_set_flip_vertically_on_load(1);
	_localBuffer = stbi_load(_filePath.c_str(), &_width, &_height, &_bitPerPixel, 4);

	glGenTextures(how_many, &_rendererId);
	glBindTexture(GL_TEXTURE_2D, _rendererId);

	// Black texture unless we don't specify these parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (_localBuffer) stbi_image_free(_localBuffer);
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