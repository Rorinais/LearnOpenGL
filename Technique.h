#pragma once
#include <glad/glad.h>
#include <list> 
#include<iostream>

class Techinque
{
public:
	Techinque();

	virtual~Techinque();

	virtual bool Init();

	void Enable(GLuint shader_program_id);

	bool AddShader(GLenum ShaderType, const char* pFilename);
	GLuint m_shaderProg;
protected:


	bool Finalize();

	GLint GetUniformLocation(const char* PUniformName);

	GLint GetProgramParam(GLint param);



private:
	typedef std::list<GLuint>ShaderObjList;

	ShaderObjList m_shaderObjList;


};