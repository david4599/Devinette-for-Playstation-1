/**********************************
**  Devinette for Playstation 1   **
**      by david4599 [2018]       **
 **********************************/


#include <stdio.h>
#include <stdlib.h>
#include "System.h"
#include <libgpu.h>

// This is a group of files we want to load
// My Data manager is set by default to load files data at address 0x80010000 which is the beginning of the user ram (see DataManager.h)
// But don't forget that we only have 2Mbytes of ram, and that our program is compiled to be loaded at the address 0x80100000 (see comp.bat)
// So this only give us 960Kbytes of free space for our data at one time ! don't forget it !

#define TENTATIVES 5

#define VALEUR_MIN 0
#define VALEUR_MAX 100

#define SCREENXRES 320
#define SCREENYRES 240


GsOT *ot;
int cputime, gputime;
int valeur, valmin = VALEUR_MIN, valmax = VALEUR_MAX, nbtentatives, recommencer, aide = 0;
unsigned int nbrandom;





void Timer(int temps) {
  int i;
  for (i=0; i<temps; i++) VSync(0);
}





int LimiterValeurMinMax(int limiteMax) {
	if (!limiteMax && (valeur <= valmin)) {
		valeur = valmin;
	}
	else if (limiteMax && (valeur >= valmax)) {
		valeur = valmax;
	}
	return valeur;
}



void AffichageCredits() {
	FntLoad(960, 0);
	FntOpen(152, 232, SCREENXRES, SCREENYRES, 0, 512);
	FntPrint("david4599 2018");
	FntFlush(-1);
	FntLoad(960, 0);
	FntOpen(0, 0, SCREENXRES, SCREENYRES, 0, 512);
}



void AffichageCommandes() {
	FntLoad(960, 0);
	FntOpen(0, 0, SCREENXRES, SCREENYRES, 0, 512);
	FntPrint("Fleche bas   : -1   Fleche haut   : +1");
	FntPrint("\nFleche gauche: -10  Fleche droite : +10");
	FntPrint("\nCTRL gauche  : -100 CTRL droite   : +100");
	FntPrint("\nTouche Croix : Valider");
	FntPrint("\nTouche Select: Recommencer");
	FntFlush(-1);
	FntLoad(960, 0);
	FntOpen(0, 0, SCREENXRES, SCREENYRES, 0, 512);
}


void AfficherAide() {
	if (aide){
		AffichageCommandes();
		if (IsPadTrig(Pad1Triangle)){
			aide = 0;
		}
	}
	else {
		FntLoad(960, 0);
		FntOpen(0, 0, SCREENXRES, SCREENYRES, 0, 512);
		FntPrint("Comment jouer ? : Touche Triangle");
		FntFlush(-1);
		FntLoad(960, 0);
		FntOpen(0, 0, SCREENXRES, SCREENYRES, 0, 512);
		if (IsPadTrig(Pad1Triangle)){
			aide = 1;
		}
	}
}


void Affichage(int valeur, int valmin, int valmax, int nbtentatives) {
	AfficherAide();
	FntPrint("\n\n\n\n\n\n\n\n\n\n\n                Essai %d", nbtentatives+1);
	FntPrint("\n\n\n      Nombre entre %d et %d : %d", valmin, valmax, valeur);
}





void Menu() {
	while (!(IsPadTrig(Pad1Start))){
		ot = System_InitFrame();
		FntPrint("\n\n\n\n\n\n\n\n\n        Devinette (plus ou moins)\n\n    Appuyer sur Start pour commencer\n");
		FntFlush(-1);
		AffichageCredits();
		System_DrawFrame(ot, &cputime, &gputime);
	}
}





void Jeu() {
	srand(GetRCnt(0));
	valmin = VALEUR_MIN;
	valmax = VALEUR_MAX;
	nbrandom = rand()%(VALEUR_MAX-VALEUR_MIN+1)+VALEUR_MIN;
	valeur = VALEUR_MIN;
	recommencer = 0;
	for (nbtentatives = 0; nbtentatives < TENTATIVES; nbtentatives++) {
		while (!IsPadTrig(Pad1Cross)){
			ot = System_InitFrame();
			Affichage(valeur, valmin, valmax, nbtentatives);
			if (IsPadPress(Pad1Down)){
				valeur--;
				valeur = LimiterValeurMinMax(0);
			}
			else if (IsPadPress(Pad1Up)){
				valeur++;
				valeur = LimiterValeurMinMax(1);
			}
			else if (IsPadPress(Pad1Left)){
				valeur-=10;
				valeur = LimiterValeurMinMax(0);
			}
			else if (IsPadPress(Pad1Right)){
				valeur+=10;
				valeur = LimiterValeurMinMax(1);
			}
			else if (IsPadTrig(Pad1L1)){
				valeur-=100;
				valeur = LimiterValeurMinMax(0);
			}
			else if (IsPadTrig(Pad1R1)){
				valeur+=100;
				valeur = LimiterValeurMinMax(1);
			}
			else if (IsPadTrig(Pad1Select)){
				recommencer = 1;
				FntFlush(-1);
				AffichageCredits();
				System_DrawFrame(ot, &cputime, &gputime);
				break;
			}
			Timer(5);
			FntFlush(-1);
			AffichageCredits();
			System_DrawFrame(ot, &cputime, &gputime);
		}
		if (recommencer) {
			break;
		}
		ot = System_InitFrame();
		if (nbrandom == valeur) {
			while (!(IsPadTrig(Pad1Start))){
				ot = System_InitFrame(); 
				Affichage(valeur, valmin, valmax, nbtentatives);
				FntPrint("\n\n\n\n                Trouve !\n\n   Appuyer sur Start pour recommencer\n");
				FntFlush(-1);
				AffichageCredits();
				System_DrawFrame(ot, &cputime, &gputime);
			}
			break;
		}
		else if (nbrandom < valeur) {
			Affichage(valeur, valmin, valmax, nbtentatives);
			FntPrint("\n\n\n\n              C'est moins\n");
			valmax = valeur;
		}
		else if (nbrandom > valeur) {
			Affichage(valeur, valmin, valmax, nbtentatives);
			FntPrint("\n\n\n\n               C'est plus\n");
			valmin = valeur;
		}
		FntFlush(-1);
		AffichageCredits();
		System_DrawFrame(ot, &cputime, &gputime);
		Timer(50);
	}

	if (nbrandom != valeur) {
		while (!(IsPadTrig(Pad1Start))){
			ot = System_InitFrame(); 
			if (recommencer) {
				Affichage(valeur, valmin, valmax, nbtentatives);
				FntPrint("\n\n\n\n         Il fallait trouver %d\n\n   Appuyer sur Start pour recommencer\n", nbrandom);
			}
			else {
				Affichage(valeur, valmin, valmax, nbtentatives-1);
				FntPrint("\n\n\n\n           Perdu ! C'etait %d\n\n   Appuyer sur Start pour recommencer\n", nbrandom);
			}
			FntFlush(-1);
			AffichageCredits();
			System_DrawFrame(ot, &cputime, &gputime);
			}
	}
}










int main(void) {
	// Init in 16bits 320x240
	System_Init(MODE_PAL, VMODE_16BITS, SCREENXRES, SCREENYRES, GsNONINTER, 1);
	System_SetBackgroundColor(100, 100, 100);
	// Init font system
	FntLoad(960, 0);
	FntOpen(0, 0, SCREENXRES, SCREENYRES, 0, 512);
	
	
	Menu();
	
	while (1) {
		Jeu();
	}
	
	System_Exit();
	return (0);
}