#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneTerrain.h"
//#include "SceneTestParallax.h"
//#include "SceneTestRefraction.h"
//#include "SceneTestFur.h"
//#include "SceneSimple.h"
//#include "SceneTestShadowMapping.h"
//#include "SceneToon.h"
#include "../Camera.h"
#include "../Config.h"
//#include "Script.h"

SceneManager::SceneManager()
{
	m_SceneDB["terrain"] = new SceneTerrain();
	//	m_SceneDB["test_parallax"] = new SceneTestParallax();
	//m_SceneDB["test_refraction"] = new SceneTestRefraction();
	//m_SceneDB["test_fur"] = new SceneTestFur();
	//	m_SceneDB["simple"] = new SceneSimple();
	//m_SceneDB["test_shadowmapping"] = new SceneTestShadowMapping();
	//m_SceneDB["toon"] = new SceneToon();

	setCurrent("terrain");
}

bool SceneManager::setCurrent(const string& name)
{
	map<string, SceneBase*>::iterator it = m_SceneDB.find(name);
	if (it == m_SceneDB.end())
		return false;

	if (m_pCurrentScene != it->second)
	{
		m_pCurrentScene = it->second;
		assert(m_pCurrentScene != NULL);
		//m_pCurrentScene->Reset();
	}
	return true;
}

SceneBase* SceneManager::getScenePointer(const string& name)
{
	map<string, SceneBase*>::iterator it = m_SceneDB.find(name);
	if (it == m_SceneDB.end())
		return NULL;
	return it->second;
}

void SceneManager::Init()
{
	for (map<string, SceneBase*>::iterator it = m_SceneDB.begin(); it != m_SceneDB.end(); it++)
		(*it).second->Init();
}

void SceneManager::Destroy()
{
	for (map<string, SceneBase*>::iterator it = m_SceneDB.begin(); it != m_SceneDB.end(); it++) {
		delete (*it).second;
		(*it).second = NULL;
	}
}

void SceneManager::Idle(float fElapsedTime)
{
	m_pCurrentScene->Idle(fElapsedTime);
}

void SceneManager::PreRender()
{
	m_pCurrentScene->PreRender();
}

void SceneManager::Render()
{
	m_pCurrentScene->Render();
}