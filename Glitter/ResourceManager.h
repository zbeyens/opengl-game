#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Singleton.h"
#include "SystemIncludes.h"

//#include "Texture2D.h"
#include "TextureCubemap.h"
//#include "Mesh.h"
#include "Shader.h"

class ResourceBase;

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	enum RES_TYPE { TEXTURE2D, TEXTURECUBEMAP, MESH, SHADER };

	void* LoadResource(RES_TYPE type, const string& name);
	void* NewResource(ResourceBase* data, const string& name);

	inline ResourceBase*	getResource(const string& name) { assert(m_ResDB.find(name) != m_ResDB.end()); return m_ResDB.find(name)->second; }
	//inline Texture2D*		getTexture2D(const string& name)		{return (Texture2D*)getResource(name);}
	inline TextureCubemap*	getTextureCubemap(const string& name)	{return (TextureCubemap*)getResource(name);}
	//inline Mesh*			getMesh(const string& name)			{return (Mesh*)getResource(name);}
	inline Shader*			getShader(const string& name) { return (Shader*)getResource(name); }

	void Reload();

	void Destroy();

protected:
	~ResourceManager() { Destroy(); }

private:
	typedef map<string, ResourceBase*> ResDB;
	ResDB	m_ResDB;
};

#endif