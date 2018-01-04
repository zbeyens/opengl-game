#ifndef TEXTURECUBEMAP_H
#define TEXTURECUBEMAP_H

#include "Texture.h"
#include "SystemIncludes.h"


// -------------------------------
// Textures Cubemap (6 images 2D)
// -------------------------------

class TextureCubemap : public Texture
{
public:
	TextureCubemap(const string& name);
	virtual GLenum getTextureType() const {return GL_TEXTURE_CUBE_MAP;}

};

#endif

