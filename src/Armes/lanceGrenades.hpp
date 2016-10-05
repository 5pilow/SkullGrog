#ifndef LANCE_GRENADES_H
#define LANCE_GRENADES_H

#include "arme.hpp"
#include "game.hpp"
#include <pge/pgeWav.h>

#include <vector>

class LanceGrenades : public Arme {
	
	
	private :
	
		// Sprites d'explosion
		std::vector<Sprite*> explosion_;
		
		// Sprite des balles
		Sprite* bulletSprite_;
		
		// bruit
		pgeWav* sound_;
		
		float damage_;
	
	
	public :
	
		LanceGrenades(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, pgeWav* sound);
		
		bool kill(float x, float y, float angle, int orientation, bool adhoc);

		void update();
};

#else


class LanceGrenades;


#endif

