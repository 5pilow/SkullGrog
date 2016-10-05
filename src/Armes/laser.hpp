#ifndef LASER_H
#define LASER_H

#include "arme.hpp"
#include "game.hpp"
#include <pge/pgeWav.h>

#include <vector>

class Laser : public Arme {
	
	
	private :
	
		// Sprites d'explosion
		std::vector<Sprite*> explosion_;
		
		// Sprite des balles
		Sprite* bulletSprite_;
		Sprite* cartoucheSprite_;
		
		// bruit
		pgeWav* sound_;
		
		float damage_;
	
	
	
	public :
	
		Laser(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound);
		
		bool kill(float x, float y, float angle, int orientation, bool adhoc);

		void update();
};

#else

class Laser;

#endif

