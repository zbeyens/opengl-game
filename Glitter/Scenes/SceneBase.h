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
	// Initialisation des donn�es propres � la sc�ne
	virtual void Init() {}

	// Rechargement des shaders
	virtual void Reload() {}

	// Destruction des donn�es propres � la sc�ne
	virtual void Destroy();

	// Evolution des donn�es et traitement des commandes synchones
	virtual void Idle(float fElapsedTime) {}

	// Traitement des commandes asynchones
	virtual void Keyboard(bool special, unsigned char key);

	// D�marrage de la sc�ne
	virtual void Reset();

	// Pr�-affichage de la sc�ne (peut utiliser des FBO)
	virtual void PreRender() {}

	// Affichage de la sc�ne (ne peut pas utiliser des FBO)
	virtual void Render() = 0;

	// On met � jour la position dans les splines de cam�ra
	void InterpolCameraTraj(float fElapsedTime);

	// Affiche la trajectoire de la cam�ra
	void DrawTraj();

	SceneBase();
	virtual ~SceneBase() { Destroy(); }

private:
	SceneBase(const SceneBase& sc) {}
	SceneBase &operator=(const SceneBase& sc) { return (*this); }

public:
	// Affiche un r�p�re
	static void DrawAxes();

	// Affiche une chaine de caract�re � l'�cran
	static void DrawString(glm::vec2 pos, void* font, const std::string& str);

	// Charge la trajectoire de la cam�ra depuis un fichier
	bool LoadCameraTrajFromFile(const std::string& name);

	// Sauve la trajectoire de la cam�ra dans un fichier
	bool SaveCameraTrajInFile(const std::string& name);

	//inline SplineGL*	getCamEyeSpline()	const { return m_pCamEyeTraj; }

protected:
	//SplineGL*		m_pCamEyeTraj;
	//SplineGL*		m_pCamLookAtTraj;
};

#endif

