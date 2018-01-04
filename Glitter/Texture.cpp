#include "Texture.h"
//#include "ImageTools.h"

bool Texture::s_bGenerateMipmaps = true;


void Texture::LoadData(GLenum target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d)
{
	glTexImage2D(target, 0, d == 3 ? GL_RGB : GL_RGBA, w, h, 0, d == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, ptr);
}

void Texture::Gen()
{
	Destroy();
	glGenTextures(1, &m_nHandle);
}

bool Texture::Load()
{
	Gen();

	if (m_nHandle == 0) {
		cerr << "Identifiant de texture incorrect" << endl;
		return false;
	}

	return true;
}

void Texture::Destroy()
{
	//glDeleteTextures(1, &m_nHandle);
	//m_nHandle = 0;
}

void Texture::Bind() const {
	glBindTexture(getTextureType(), m_nHandle);
}

void Texture::Bind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glEnable(getTextureType());
	glBindTexture(getTextureType(), m_nHandle);
}

void Texture::Unbind() const {
	glBindTexture(getTextureType(), 0);
}

void Texture::Unbind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(getTextureType(), 0);
}