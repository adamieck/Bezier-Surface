#pragma once
#include <string>

class Texture
{
private:
	unsigned int _rendererID;
	std::string _filepath;
	unsigned char* _buffer;
	int _width, _height, _bitsPerPic;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
};
