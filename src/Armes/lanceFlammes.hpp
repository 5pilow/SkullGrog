#ifndef LANCEFLAMMES_H
#define LANCEFLAMMES_H

#include "sprite.hpp"
#include "game.hpp"
#include <pge/pgeWav.h>
#include "arme.hpp"

class LanceFlammes : public Arme {
	
	private :
	
		Sprite* fireSprite_;
		
		pgeWav* sound_;
		
		int damageParticle_;
	
	
	public :
	
		LanceFlammes(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, pgeWav* sound);
		~LanceFlammes();
		
		bool kill(float x, float y, float angle, int orientation, bool adhoc);
		
		void update();
	
	
	
};

#else

class LanceFlammes;

#endif
