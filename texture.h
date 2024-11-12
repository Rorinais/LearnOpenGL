#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#ifndef TEXTURE_H
#define TEXTURE_H

class Texture 
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);
	~Texture();
	bool load();

	void Bind(GLenum TextureUnit);
private:
	std::string m_filename;
	GLenum m_textureTarget;
	GLuint m_textureObj;

};


#endif // !TEXTURE
