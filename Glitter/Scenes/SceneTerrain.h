#pragma once

#ifndef SCENETERRAIN_H
#define SCENETERRAIN_H

#include "../SystemIncludes.h"
#include "SceneBase.h"
//#include "../FrameBufferObject.h"

#define TERRAIN_SHADOWMAPS_COUNT 2

class Texture2D;
class TextureCubemap;
class Shader;
class Terrain;

class SceneTerrain : public SceneBase
{
public:
	SceneTerrain() {};
	virtual void Init();
	virtual void Reload();
	virtual void Destroy();
	virtual void PreRender();
	virtual void Render();
	virtual void Idle(float fElapsedTime);

private:
	// On place les matrices et la cam�ra au point de vue de la lumi�re pour le pr�-rendu
	void SetLightCameraMatrices();
	void RestoreLightCameraMatrices();

	// Rendu de l'environnement (pour la r�flexion de l'eau ou non)
	void RenderEnvironment(bool bWaterReflection, bool bDepthMap);

	// Rendu de la surface de l'eau
	void RenderWaterSurface();

public:
	inline void	setSunAngle(const glm::vec2& a) { m_vSunAngle = a; }

private:
	//Terrain*				m_pTerrain;
	glm::vec2					m_vSunAngle;
	glm::vec4					m_vSunVector;
	glm::mat4					m_matSunModelviewProj;

	TextureCubemap*			m_pSkybox;

	//Shader*					m_pShaderLighting;
	//Shader*					m_pShaderTerrain;
	//Shader*					m_pShaderWater;
	//Shader*					m_pShaderGrass;
	//Shader*					m_pShaderTree;
	//Texture2D*				m_pTerrainDiffuseMap;
	//std::vector<Texture2D*>	m_tTextures;

	//Texture2D*				m_pTexWaterNoiseNM;
	//FrameBufferObject		m_fboWaterReflection;

	//FrameBufferObject		m_fboDepthMapFromLight[TERRAIN_SHADOWMAPS_COUNT];

	unsigned int skyboxVAO, skyboxVBO;

};

#endif