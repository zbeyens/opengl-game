#ifndef TIMER_H
#define TIMER_H

// -------------------------------
// Gestion du temps
// -------------------------------

#include "SystemIncludes.h"

double	startTime;		// date du système lors de l'initialisation de la base de temps
double	currentTime;	// Temps courant remis à jour à chaque frame
double	elapsedTime;
int		nFPS;

void timerStart(double time);
void timerIdle(double time);

void timerIdle(double time) {
	static int tempFPS = 0;
	static double fPreviousFPSTime = 0.0f;


	double fNewCurrentTime = time - startTime;


	// Measure speed
	elapsedTime = fNewCurrentTime - currentTime;
	tempFPS++;
	if (fNewCurrentTime - fPreviousFPSTime >= 1.0f) { // If last prinf() was more than 1 sec ago
													  // printf and reset timer
		fPreviousFPSTime = fNewCurrentTime;
		nFPS = tempFPS;
		cout << 1000.0 / double(nFPS) << " ms/frame -> " << nFPS << " frames/sec" << endl;
		tempFPS = 0;
	}

	currentTime = fNewCurrentTime;
}

void timerStart(double time) {
	startTime = time;
	currentTime = 0.0f;
	elapsedTime = 0.0f;
	nFPS = 0;
}


#endif