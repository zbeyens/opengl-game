#ifndef SCENEBASE_H
#define SCENEBASE_H

#ifdef WIN32
#include <windows.h>
#endif

//#include "Mathlib.h"
#include "../SystemIncludes.h"

//class SplineGL;

// Classe de base abstraite des sc�nes
class SceneBase
{
public:
	SceneBase() {};
	// Initialisation des donn�es propres � la sc�ne
	virtual void Init() {}

	// Rechargement des shaders
	virtual void Reload() {}

	// Evolution des donn�es et traitement des commandes synchones
	virtual void Idle(float fElapsedTime) {}

	// Pr�-affichage de la sc�ne (peut utiliser des FBO)
	virtual void PreRender() {}

	// Affichage de la sc�ne (ne peut pas utiliser des FBO)
	virtual void Render() = 0;

private:
	SceneBase(const SceneBase& sc) {}
	SceneBase &operator=(const SceneBase& sc) { return (*this); }

public:
};

#endif

