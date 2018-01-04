#ifndef VBO_H
#define VBO_H

#include "SystemIncludes.h"
//#include "Mathlib.h"

class VertexBufferObject
{
public:
	bool		Create(GLenum usage);
	void		Destroy();

	// Activation et désactivation du VB
	void		Enable();
	void		Disable();

	// Buffers de données
	inline vector<vec3>&	getPosition()	{return m_tDataPosition;}
	inline vector<vec3>&	getNormal()		{return m_tDataNormal;}
	inline vector<vec2>&	getTexcoord()	{return m_tDataTexcoord;}
	inline vector<vec3>&	getTangent()	{return m_tDataTangent;}

	VertexBufferObject();
	~VertexBufferObject() {Destroy();}

private:
	void Enable_VA();	// Activation en Vertex Array
	void Enable_VBO();	// Activation en Vertex Buffer Object
	void Disable_VA();	// Désactivation en Vertex Array
	void Disable_VBO();	// Désactivation en Vertex Buffer Object

private:
	// Identifiants pour le VBO
	GLuint		m_nVBOid;
	GLintptr	m_nVBO_OffsetPosition;
	GLintptr	m_nVBO_OffsetNormal;
	GLintptr	m_nVBO_OffsetTexcoord;
	GLintptr	m_nVBO_OffsetTangent;

	// Données :
	vector<vec3>	m_tDataPosition;
	vector<vec3>	m_tDataNormal;
	vector<vec2>	m_tDataTexcoord;
	vector<vec3>	m_tDataTangent;
};

#endif

