#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Singleton.h"
#include "../SystemIncludes.h"


class SceneBase;

SINGLETON_BEGIN(SceneManager)
public:
	SceneManager();

	void Init();
	void Destroy();
	void Idle(float fElapsedTime);
	void Keyboard(bool special, unsigned char key);
	void PreRender();
	void Render();

	bool setCurrent(const string& name);
	SceneBase* getScenePointer(const string& name);
	inline SceneBase* getCurrentScenePointer()	const { return m_pCurrentScene; }

private:
	map<string, SceneBase*>	m_SceneDB;			// Toutes les scènes
	SceneBase*							m_pCurrentScene;	// Pointeur sur la scène courante

	SINGLETON_END()

#endif
