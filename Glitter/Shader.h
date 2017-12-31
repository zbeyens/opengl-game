#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "SystemIncludes.h"



// Simple shader class from http://www.learnopengl.com/ with a few tweaks
class Shader {
public:
	// State
	GLuint ID;

	// Constructor
	Shader(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr, const GLchar *tessCPath = nullptr, const GLchar *tessEPath = nullptr);

	// Sets the current shader as active, do we need to return?
	Shader& use();

	// Not sure compile should be it's own step separate from constructor
	void compile();

	void setFloat(const GLchar *name, GLfloat value);
	void setInt(const GLchar *name, GLint value);

	void setVec2(const GLchar *name, GLfloat x, GLfloat y);
	void setVec2(const GLchar *name, const glm::vec2 &value);

	void setVec3(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
	void setVec3(const GLchar *name, const glm::vec3 &value);

	void setVec4(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setVec4(const GLchar *name, const glm::vec4 &value);

	void setMat4(const GLchar *name, const glm::mat4 &matrix);
private:
	// Checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(const GLuint &object, std::string type);

	// Make a shader from a filepath
	GLuint pathToShader(const GLchar * path, GLenum shaderType);

	// Get a string of the shader type from the GLenum
	std::string shaderTypeToString(GLenum shaderType);

	const GLchar* mVertexPath;
	const GLchar* mFragmentPath;
	const GLchar* mGeometryPath;
	const GLchar* mTessCPath;
	const GLchar* mTessEPath;
};

#endif