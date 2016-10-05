#ifndef GAZEUR_H
#define GAZEUR_H

#include "arme.hpp"
#include "sprite.hpp"
#include "game.hpp"
#include <pge/pgeWav.h>


class Gazeur : public Arme {
	
	private :
	
		Sprite* fireSprite_;
		
		pgeWav* sound_;
		
		int damageParticle_;
	
	
	public :
	
		Gazeur(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, pgeWav* sound);
		~Gazeur();
		
		bool kill(float x, float y, float angle, int orientation, bool adhoc);
		
		void update();
	
	
	
};

#else

class Gazeur;

#endif
