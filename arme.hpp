#ifndef ARME_H
#define ARME_H

#include <pge/pgeTexture.h>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>

class Particles;
#include "sprite.hpp"
class Game;



class Arme {
	
	protected :

		int id_;
		
		// système de particules
		Particles* pSystem_;
	
		// sprite de l'arme
		Sprite* sprite_;
		
		// le jeu
		Game* game_;
		
		// dimensions de l'arme
		float width_;
		float height_;
		float widthTir_;
		
		// Position des mains
		ScePspFVector2 main1_;
		ScePspFVector2 main2_;
		
		unsigned int chargeur_;
		unsigned int munitions_;
		unsigned int surchauffe_;
		float shootDelay_;
		float shootDelayCurrent_;

		float angle_;

		bool tirTrue_;
		
	public :
	
		Arme(Particles* pSystem, Sprite* sprite);
		~Arme();
		
		int getWidth();
		int getHeight();
		int getWidthTir();
		float getAngle();
		bool getTirTrue();
		Sprite* getSprite();
		int getMunitions();
		int getChargeur();
		int getID();
		
		void setGame(Game* game);

		void noDelay();
		
		void addMunitions();
		
		ScePspFVector2 getPosMain(int main);
		
		void draw(float x, float y, float angle, unsigned char alpha, bool reverse);
		
		virtual bool kill(float x , float y, float angle, int orientation, bool adhoc);
		
		virtual void update();
};

#else

class Arme;

#endif
