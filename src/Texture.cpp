#include "Texture.h"
#include "stb/stb_image.h"
#include <glad/glad.h>

Texture::Texture(const std::string& path)
	: _filepath(path), _buffer(nullptr),
	_width(0), _height(0), _bitsPerPic(0)
{
	stbi_set_flip_vertically_on_load(1);
	_buffer = stbi_load(path.c_str(), &_width, &_height,
		&_bitsPerPic, 4);


	glGenTextures(1, &_rendererID);
	glBindTexture(GL_TEXTURE_2D, _rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (_buffer)
		stbi_image_free(_buffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_rendererID);
}

void Texture::Bind(unsigned slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);

}
