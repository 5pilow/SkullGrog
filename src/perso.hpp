#ifndef PERSO_H
#define PERSO_H

#include <pge/pgeControls.h>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>
#include <pge/pgeWav.h>
#include <vector>

#include "particles.hpp"
//~ class Particles;
#include "map.hpp"
#include "sprite.hpp"
#include "utils.hpp"


enum PersoTeam { HUMAN, ZOMBIE };

class Perso {
	
	protected :
	
		Particles* pSystem_;
		Map* map_;
		
		Sprite* headSprite_;
		Sprite* bodySprite_;
		Sprite* footSprite_;
		Sprite* main1Sprite_;
		Sprite* main2Sprite_;
		Sprite* bloodSprite_;
		Sprite* bloodSprite2_;
		
		ScePspFVector2 pos_;
		float speed_;
		float angleViseur_;
		short orientation_;
		float jump_;
		int numJump_;
		float walkFrame_;
		float breathFrame_;
		float weaponRecul_;
		int life_;
		bool alive_;
		int burning_;
		
		PersoTeam team_;
		
		
	public :
	
		Perso(float x, float y, std::vector<Sprite*>& sprites, Particles* pSystem, Map* map);
		~Perso();

		void reset();
		
		ScePspFVector2 getPos();
		ScePspFVector2* getPosPtr();
		
		int getOrientation();
		PersoTeam getTeam();
		
		virtual void inflictDamage(float damage,float x, float y, float angle);
		void burn();
		
		void update();
		
		int getLife();
		float getAngleViseur();
		float getWalkFrame();
		
		
		void setPos(float x, float y);
		void setLife(int life);
		void setAngleViseur(float angle);
		void setWalkFrame(float walkFrame);
		
		void draw();
		void drawBody();
		
		void move(float x, float y);
	
};

#else

class Perso;

#endif
