#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "SystemIncludes.h"
#include "ResourceBase.h"

// Simple shader class from http://www.learnopengl.com/ with a few tweaks
class Shader : public ResourceBase
{
public:
	// Constructor
	Shader(const string& name);//, const string fragmentSource, const string geometrySource = nullptr, const string tessCPath = nullptr, const string tessEPath = nullptr);
	
							   //bool Load(const string& name);
	void Destroy();

	bool Activate();
	void Deactivate();

	//static Shader* getActivatedShader() { return s_pCurrent; }


	// Sets the current shader as active, do we need to return?
	//Shader& use();

	void setFloat(const GLchar *name, GLfloat value);
	void setInt(const GLchar *name, GLint value);

	void setVec2(const GLchar *name, GLfloat x, GLfloat y);
	void setVec2(const GLchar *name, const glm::vec2 &value);

	void setVec3(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
	void setVec3(const GLchar *name, const glm::vec3 &value);

	void setVec4(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setVec4(const GLchar *name, const glm::vec4 &value);

	void setMat4(const GLchar *name, const glm::mat4 &matrix);

	GLuint		m_nProgram;


private:
	//static bool		s_bInitialized;
	//static Shader*	s_pCurrent;
	string		m_strName;

	// Checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(const GLuint &object, string type);

	// Make a shader from a filepath
	GLuint pathToShader(const string path, GLenum shaderType);

	// Get a string of the shader type from the GLenum
	string shaderTypeToString(GLenum shaderType);

	string mVertexPath;
	string mFragmentPath;
	string mGeometryPath;
	string mTessCPath;
	string mTessEPath;
};

#endif