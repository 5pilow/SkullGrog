#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <pge/pgeControls.h>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>
#include <pge/pgeWav.h>
#include <pspdebug.h>
#include <pspkernel.h>

#include "perso.hpp"
#include "particles.hpp"




class Zombie : public Perso {
	
	private :
	
		std::vector<pgeWav*> screams_;

		bool finish_;
		bool dead_;

		std::vector<Sprite*> bouts_;
				
		ScePspFVector2 target_;
		
		std::vector<Tile> chemin_;
		int searchPath_;
		short dirX_;
		short dirY_;
		short nextDirX_;

		bool attack_;
		float attackFrame_;
		float deadFrame_;
		
		
	public :
	
		Zombie(float x, float y, std::vector<Sprite*>& sprites, std::vector<Sprite*>& bouts, Particles* pSystem, Map* map, std::vector<pgeWav*>& cris);
		~Zombie();
		
		void setTarget(ScePspFVector2 target);
		void setSpeed(float speed);
		
		void reset();

		bool isDead();
		bool isFinish();

		void inflictDamage(float damage,float x, float y, float angle);

		void move();
		
		void draw();
		
		
		
		
	
};

#else

class Zombie;

#endif
