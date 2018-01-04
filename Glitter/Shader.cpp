#include "Shader.h"



Shader::Shader(const string& name)//, const string fragmentPath, const string geometryPath, const string tessCPath, const string tessEPath) 
{
	m_strName = name;

	//mFragmentPath = fragmentPath;
	//mGeometryPath = geometryPath;
	//mTessCPath = tessCPath;
	//mTessEPath = tessEPath;

	GLuint vertexShader = pathToShader("./resources/shaders/" + m_strName + ".vert", GL_VERTEX_SHADER);
	GLuint fragmentShader = pathToShader("./resources/shaders/" + m_strName + ".frag", GL_FRAGMENT_SHADER);
	//GLuint geometryShader = (mGeometryPath)? pathToShader(mGeometryPath, GL_GEOMETRY_SHADER) : 0;
	//GLuint tessCShader = (mTessCPath) ? pathToShader(mTessCPath, GL_TESS_CONTROL_SHADER) : 0;
	//GLuint tessEShader = (mTessEPath) ? pathToShader(mTessEPath, GL_TESS_EVALUATION_SHADER) : 0;

	// Create Program
	m_nProgram = glCreateProgram();

	// Attach the shaders to the program
	glAttachShader(m_nProgram, vertexShader);
	glAttachShader(m_nProgram, fragmentShader);
	//if (mGeometryPath) glAttachShader(m_nProgram, geometryShader);
	//if (mTessCPath) glAttachShader(m_nProgram, tessCShader);
	//if (mTessEPath) glAttachShader(m_nProgram, tessEShader);


	glLinkProgram(m_nProgram);
	checkCompileErrors(m_nProgram, "Program");

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//if (mGeometryPath) glDeleteShader(geometryShader);
	//if (mTessCPath) glDeleteShader(tessCShader);
	//if (mTessEPath) glDeleteShader(tessEShader);
}


//Shader &Shader::use() {
//	glUseProgram(m_nProgram);
//	return *this;
//}

bool Shader::Activate()
{
	if (!m_nProgram) {
		return false;
	}

	//s_pCurrent = this;
	glUseProgram(m_nProgram);
	return true;
}


void Shader::setFloat(const GLchar *name, GLfloat value) {
	glUniform1f(glGetUniformLocation(m_nProgram, name), value);
}
void Shader::setInt(const GLchar *name, GLint value) {
	glUniform1i(glGetUniformLocation(m_nProgram, name), value);
}
void Shader::setVec2(const GLchar *name, GLfloat x, GLfloat y) {
	glUniform2f(glGetUniformLocation(m_nProgram, name), x, y);
}
void Shader::setVec2(const GLchar *name, const glm::vec2 &value) {
	glUniform2f(glGetUniformLocation(m_nProgram, name), value.x, value.y);
}
void Shader::setVec3(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) {
	glUniform3f(glGetUniformLocation(m_nProgram, name), x, y, z);
}
void Shader::setVec3(const GLchar *name, const glm::vec3 &value) {
	glUniform3f(glGetUniformLocation(m_nProgram, name), value.x, value.y, value.z);
}
void Shader::setVec4(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	glUniform4f(glGetUniformLocation(m_nProgram, name), x, y, z, w);
}
void Shader::setVec4(const GLchar *name, const glm::vec4 &value) {
	glUniform4f(glGetUniformLocation(m_nProgram, name), value.x, value.y, value.z, value.w);
}
void Shader::setMat4(const GLchar *name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(m_nProgram, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(const GLuint &object, string type) {
	GLint success;
	GLchar infoLog[1024];
	
	if (type != "Program") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			cout << "| ERROR::SHADER: Compile-time error: Type: " << type << endl;
			cout << infoLog << "\n -- --------------------------------------------------- -- \n";
		}
	} else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			cout << "| ERROR::Shader: Link-time error: Type: " << type << endl;
			cout << infoLog << "\n -- --------------------------------------------------- -- \n";
		}
	}
}

GLuint Shader::pathToShader(const string path, GLenum shaderType) {
	ifstream shaderFile;
	stringstream shaderStream;

	shaderFile.open(path);
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	string code = shaderStream.str();
	const GLchar * ccode = code.c_str();

	// Vertex Shader
	GLuint shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &ccode, NULL);
	glCompileShader(shader);
	checkCompileErrors(shader, shaderTypeToString(shaderType));

	return shader;
}

string Shader::shaderTypeToString(GLenum shaderType) {
	string ret = "";

	switch (shaderType) {
	case GL_COMPUTE_SHADER:
		ret = "Compute";
		break;
	case GL_VERTEX_SHADER:
		ret = "Vertex";
		break;
	case GL_TESS_CONTROL_SHADER:
		ret = "Tessellation Control";
		break;
	case GL_TESS_EVALUATION_SHADER:
		ret = "Tessellation Evaluation";
		break;
	case GL_GEOMETRY_SHADER:
		ret = "Geometry";
		break;
	case GL_FRAGMENT_SHADER:
		ret = "Fragment";
		break;
	default:
		ret = "unknown";
		break;
	}
	return ret;
}


void Shader::Destroy()
{
	Deactivate();
	m_nProgram = 0;
}

void Shader::Deactivate()
{
	//s_pCurrent = NULL;
	glUseProgram(0);
}