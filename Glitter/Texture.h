#ifndef TEXTURE_H
#define TEXTURE_H


#include "ResourceBase.h"
#include "SystemIncludes.h"

// -------------------------------
// Abstract Texture
// -------------------------------

class Texture : public ResourceBase
{
public:
	virtual GLenum getTextureType() const = 0;

	void Gen();
	bool Load();
	virtual void Destroy();

	void Bind(GLuint slot) const;
	void Unbind(GLuint slot) const;

	GLuint getHandle() const {return m_nHandle;} 

	static void EnableGenerateMipmaps(bool b) {s_bGenerateMipmaps=b;}

	Texture() {m_nHandle=0;}
	~Texture() {Destroy();}
	void Bind() const;
	void Unbind() const;
	void LoadData(GLenum target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d);

protected:
	static bool s_bGenerateMipmaps;	// generate mipmap or not
	GLuint	m_nHandle;				// texture opengl id
};

#endif

