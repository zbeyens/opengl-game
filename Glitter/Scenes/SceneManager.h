#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Singleton.h"
#include "../SystemIncludes.h"

class SceneBase;

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
public:
	SceneManager();

	void Init();
	void Destroy();
	void Idle(float fElapsedTime);
	void PreRender();
	void Render();

	bool setCurrent(const string& name);
	SceneBase* getScenePointer(const string& name);
	inline SceneBase* getCurrentScenePointer()	const { return m_pCurrentScene; }

private:
	map<string, SceneBase*>	m_SceneDB;			// Toutes les scènes
	SceneBase*				 m_pCurrentScene;	// Pointeur sur la scène courante
};

#endif
