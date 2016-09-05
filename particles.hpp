#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdio.h>
#include <vector>
#include <pge/pgeTexture.h>
#include <pge/pgeMath.h>
#include <pge/pgeWav.h>
#include <pspgu.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <math.h>
#include <stack>

#include "sprite.hpp"
#include "map.hpp"

class Zombie;
class Human;

enum ParticleType { FIRE, EXPLOSION, BULLET, CARTOUCHE, BLOOD, LASER, GRENADE, GAZ, BOUT, CENDRE};

typedef struct Particle {
	
	float x, y;
	float dx, dy;
	float angle;
	float speed;
	float size;
	int life;
	ParticleType type;
	Sprite* sprite;
	float damage;
	bool adhoc;
	
} Particle;

typedef struct LaserParticle {
	std::vector<pgeVertV> vertices;
	float x,y;
	float dx,dy;
	float life;
	float damage;
	float angle;
	Sprite* sprite;
	bool adhoc;

} LaserParticle;

	
class Particles {
	
	private :
		
		std::vector<Particle> particles_;
		std::vector<LaserParticle> lasers_;
		
		std::vector<Sprite*> sprites_;
		std::vector<pgeWav*> sounds_;
		Sprite* explosionSprite_;

		Map* map_;
		
		float scale_;
		ScePspFVector2 camera_;
		
	public :
		
	
		Particles(Map* map, Sprite* explosionSprite);
		~Particles();

		void reset();
		
		void addParticle(float x, float y, float angle, float dx, float dy, float size, ParticleType type, Sprite* sprite,float damage, bool adhoc);
		void addParticleFond(Sprite* sprite,float x, float y, float angle, char alpha);

		void addLaser(float x, float y, float dx, float dy, float damage, float angle, Sprite* sprite, bool adhoc);
		
		void addSprite(Sprite* sprite);
		void addSound(pgeWav* sound);
		
		int getNumParticles();
		void setCamera(ScePspFVector2 camera);
		
		void update();
		void collide(std::vector<Zombie*>& zombies);
		void collide(Human* human);
		void collideAdhoc(Human* human);
		
		void draw();
	
	
};

#else

class Particles;

#endif
