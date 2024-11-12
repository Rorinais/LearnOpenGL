#include"Technique.h"
#include<iostream>
using namespace std;
#include "Utils.h"

Techinque::Techinque() {
	m_shaderProg = 0;
}

Techinque::~Techinque() {
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	{
		glDeleteShader(*it);
	}
	if (m_shaderProg != 0)
	{
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool Techinque::Init() {

	m_shaderProg = glCreateProgram();
	if(m_shaderProg==0)
	{ 
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;
}
bool Techinque::AddShader(GLenum ShaderType, const char* pFilename) {

	string s;
	if (!ReadFile(pFilename, s)) {
		return false;
	}

	GLuint shaderObj = glCreateShader(ShaderType);
	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type%d\n",ShaderType);
		return false;
	}

	m_shaderObjList.push_back(shaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };

	glShaderSource(shaderObj, 1, p, Lengths);
	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling  '%s': '%s'\n", pFilename, infoLog);
		return false;
	}

	glAttachShader(m_shaderProg, shaderObj);
	return true;

}

bool Techinque::Finalize() {
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}


	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		return false;
	}

	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}
	m_shaderObjList.clear();
	return GLCheckError();
}


void Techinque::Enable(GLuint shader_program_id) {
	glUseProgram(shader_program_id);
}


GLint Techinque::GetUniformLocation(const char* PUniformName) {

	GLuint Location = glGetUniformLocation(m_shaderProg, PUniformName);
	
	if (Location == -1)
	{
		fprintf(stderr, "Warning!Unable to get the location of uniform %s\n", PUniformName);
	}
	return Location;
}
GLint Techinque::GetProgramParam(GLint param) {

	GLint ret;
	
	glGetProgramiv(m_shaderProg, param, &ret);

	return ret;
}