#include "particles.hpp"
#include "zombie.hpp"

//~ class Human;
#include "human.hpp"
//~ class Sprite;
//~ class Perso;
#include "perso.hpp"
//~ class Map;
class Zombie;

#define MAX_PARTICLES 500
#define PAIR(x) (x/2) == (int)(x/2)

std::vector<Particle> toAdd;

Particles::Particles(Map* map, Sprite* explosionSprite) {

	map_ = map;
	scale_ = 1.0f;
	camera_ .x = 0;
	camera_ .y = 0;
	
	explosionSprite_ = explosionSprite;
}

void Particles::reset() {

	particles_.clear();
}

void Particles::addParticle(float x, float y, float angle, float dx, float dy, float size,ParticleType type, Sprite* sprite, float damage, bool adhoc) {

	if (map_->collideX(x,y) || particles_.size() >= MAX_PARTICLES) return;
	
	int life;
	float speed;
	
	switch (type) {
		case (CARTOUCHE) :
			life = 300; speed = 5; 
			break;
		case (BOUT) :
			life = 300; speed = 5; 
			break;
		case (GRENADE) :
			life = 500; speed = 5; 
			break;
		case (FIRE) :
			life = 50; speed = pgeMathRandFloat(5,8); sprite = sprites_[0];
			break;
		case (GAZ) :
			life = 50; speed = pgeMathRandFloat(5,8); sprite = sprites_[0];
			break;
		case (CENDRE) :
			life = 200; speed = 3;
			break;
		case (BULLET) :
			life = 30; speed = 20;
			break;
		case (EXPLOSION) :
			life = 15; speed = 0;
			break;
		case (BLOOD) :
			life = 15; speed = 3; 
			break;
		case (LASER) :
			life = 15; speed = 0; 
	}
		
	Particle p = {x,y, dx,dy, angle, speed, size,life, type, sprite, damage, adhoc};
	toAdd.push_back(p);
}

void Particles::addParticleFond(Sprite* sprite, float x, float y, float angle, char alpha) {

	map_->addParticleFond(sprite, x,y,angle,alpha);
}

void Particles::addLaser(float x, float y, float dx, float dy, float damage, float angle, Sprite* sprite, bool adhoc) {

	if (map_->collideX(x,y)) return;

	std::vector<pgeVertV> vertices;

	pgeVertV v = {x,y};
	vertices.push_back(v);
	
	LaserParticle laser = {vertices, x,y, dx, dy, 50, damage, angle, sprite, adhoc};
	lasers_.push_back(laser);
}

void Particles::setCamera(ScePspFVector2 camera) {
	
	camera_ = camera;
}

void Particles::addSprite(Sprite* sprite) {
	
	sprites_.push_back(sprite);
}

void Particles::addSound(pgeWav* sound) {
	
	sounds_.push_back(sound);
}

int Particles::getNumParticles() {
	
	return (int) particles_.size();
}

void Particles::update() {

	for (int i = 0; i < toAdd.size(); i++) {

		particles_.push_back(toAdd[i]);
	}
	toAdd.clear();
	
	for (std::vector<Particle>::iterator p = particles_.begin(); p != particles_.end(); ++p) {

		// move
		float dx = 0, dy = 0;

		dx = p->dx * p->speed;
		p->x += dx;

		
		if (p->type == CARTOUCHE || p->type == GRENADE || p->type == BOUT || p->type == CENDRE) {
			/// collide x
			if (dx != 0) {
				if (map_->collideX(p->x, p->y)) {
					p->x -= dx;
					p->dx *= -0.7f;
					if (p->type != BOUT)
						pgeWavPlay(sounds_[0]);
				}
			}
		}

		dy = p->dy * p->speed;
		p->y += dy;
		
		if (p->type == CARTOUCHE || p->type == GRENADE || p->type == BOUT || p->type == CENDRE) {
			
			/// collide y
			if (dy != 0) {
				if (map_->collideX(p->x, p->y)) {
					p->y -= dy;
					if (p->type == CARTOUCHE || p->type == GRENADE) {
						p->dy *= -0.5f;
					} else {
						p->dy *= -0.3f;
					}
					p->dx *= 0.5f;
					if (p->type != BOUT && p->type != CENDRE)
						pgeWavPlay(sounds_[0]);

					if (pgeMathAbs(p->dy) < 0.1f && dy > 0 && pgeMathAbs(p->dx) < 0.1f) {
						// finit
						if (p->type == GRENADE) {
							p->type = EXPLOSION;
							p->y -= 50;
							p->sprite = explosionSprite_;
							p->life = 30;
							p->damage = 100;
							p->angle = 0;
							p->dx = 0;
							p->dy = 0;
							
							// bruit d'explosion
							pgeWavPlay(sounds_[1]);
						} else if (p->type == CENDRE) {
							particles_.erase(p);
							--p; 
						} else {
							map_->addParticleFond(p->sprite, p->x, p->y, p->angle, 255);
							particles_.erase(p);
							--p; 
							
						}
						continue;
					}
				}
			}

			// gravité
			p->dy += 9.81 / 300;

			// rotation
			if (p->type == CARTOUCHE || p->type == GRENADE) {
				p->angle += 0.1f;
			} else {
				p->angle += 0.05f;
			}
			
		} else {
			
			// si ça touche un mur on l'enleve
			if (map_->collideX(p->x, p->y)) {
				particles_.erase(p);
				--p; 
				continue;
			}
		}
		
		if (p->type == FIRE || p->type == GAZ) {
			p->dy -= 0.01;
		}
		
		p->life--;
		if (p->life <= 0) {
			
			particles_.erase(p);
			--p;
			continue;
			
		}
		
	
	}

	for (std::vector<LaserParticle>::iterator l = lasers_.begin(); l != lasers_.end(); ++l) {

		l->life--;
		if (l->life <= 0) {
			lasers_.erase(l);
			--l;
			continue;
		}
		
		int speed = 10;
		
		// move
		for (int i = 0; i < 3; i++) {
			float dx = l->dx * speed;
			l->x += dx;
			/// collide x
			if (dx != 0) {
				if (map_->collideX(l->x, l->y)) {
					l->x -= dx;
					l->dx *= -1;

					pgeVertV v = {l->x, l->y};
					l->vertices.push_back(v);
				}
			}

			float dy = l->dy * speed;
			l->y += dy;
			/// collide y
			if (dy != 0) {
				if (map_->collideX(l->x, l->y)) {
					l->y -= dy;
					l->dy *= -1;

					pgeVertV v = {l->x, l->y};
					l->vertices.push_back(v);
				}
			}
		}
	}
}

void Particles::collide(std::vector<Zombie*>& zombies) {

	for (std::vector<Particle>::iterator p = particles_.begin(); p != particles_.end(); ++p) {
		/// Apply damages
		if (p->damage > 0) {
			for (int j = 0; j < (int) zombies.size() ; j++) {

				if (zombies[j]->isDead()) continue;

				if (p->type != LASER) {
					if ( pgeMathSqrt(pow(p->x - zombies[j]->getPos().x,2) + pow(p->y - ( zombies[j]->getPos().y - 50),2) ) < 40) {
						
						// kill the perso
						zombies[j]->inflictDamage(p->damage, p->x, p->y, p->angle);
						if (p->type == FIRE) zombies[j]->burn();
						
						// erase the particle
						if (p->type != FIRE && p->type != EXPLOSION && p->type != GAZ) {
							particles_.erase(p);
							--p;
							break;
						}
					}
				} else {
					float cs = pgeMathCos(p->angle);
					float sn = pgeMathSin(p->angle);
					
					// laser, on vérifie tout le long du laser
					for (float i = -p->size/2; i < p->size/2; i += p->size/20) {

						float x = p->x + cs * i;
						float y = p->y + sn * i;

						if ( pgeMathSqrt(pow(x - zombies[j]->getPos().x,2) + pow(y - ( zombies[j]->getPos().y - 50),2) ) < 40) {

							// kill the perso
							zombies[j]->inflictDamage(p->damage, x, y, p->angle);
						}
					}
				}
			}
		}
	}

	for (std::vector<LaserParticle>::iterator l = lasers_.begin(); l != lasers_.end(); ++l) {

		/// Apply damages
		if (l->damage > 0) {
			for (int j = 0; j < (int) zombies.size() ; j++) {

				if ( pgeMathSqrt(pow(l->x - zombies[j]->getPos().x,2) + pow(l->y - ( zombies[j]->getPos().y - 50),2) ) < 30) {
					
					// kill the perso
					zombies[j]->inflictDamage(l->damage, l->x, l->y, l->angle);
				}
			}
		}
	}
}

void Particles::collide(Human* human) {

	for (std::vector<Particle>::iterator p = particles_.begin(); p != particles_.end(); ++p) {
		/// Apply damages
		if (p->damage > 0) {
			if (p->type != LASER) {
				if ( p->adhoc == true && pgeMathSqrt(pow(p->x - human->getPos().x,2) + pow(p->y - ( human->getPos().y - 50),2) ) < 40) {
					
					// kill the perso
					human->inflictDamage(p->damage, p->x, p->y, p->angle);
					if (p->type == FIRE) human->burn();
					
					// erase the particle
					if (p->type != FIRE && p->type != EXPLOSION && p->type != GAZ) {
						particles_.erase(p);
						--p;
					} else {
						p->damage = 0;
					}
				}
			} else if (p->adhoc == true ) {
				float cs = pgeMathCos(p->angle);
				float sn = pgeMathSin(p->angle);
				
				// laser, on vérifie tout le long du laser
				for (float i = -p->size/2; i < p->size/2; i += p->size/20) {

					float x = p->x + cs * i;
					float y = p->y + sn * i;

					if ( pgeMathSqrt(pow(x - human->getPos().x,2) + pow(y - ( human->getPos().y - 50),2) ) < 40) {

						// kill the perso
						human->inflictDamage(p->damage, x, y, p->angle);
						p->damage = 0;
					}
				}
			}
		}
	}

	for (std::vector<LaserParticle>::iterator l = lasers_.begin(); l != lasers_.end(); ++l) {

		/// Apply damages
		if (l->damage > 0 && l->adhoc == true) {

			if ( pgeMathSqrt(pow(l->x - human->getPos().x,2) + pow(l->y - ( human->getPos().y - 50),2) ) < 30) {
				// kill the perso
				human->inflictDamage(l->damage, l->x, l->y, l->angle);
				return;
			}
		}
	}
}

void Particles::collideAdhoc(Human* human) {

	for (std::vector<Particle>::iterator p = particles_.begin(); p != particles_.end(); ++p) {
		/// Apply damages
		if (p->damage > 0) {
			if (p->type != LASER) {
				if ( p->adhoc == false && pgeMathSqrt(pow(p->x - human->getPos().x,2) + pow(p->y - ( human->getPos().y - 50),2) ) < 40) {
					
					// kill the perso
					human->inflictDamage(p->damage, p->x, p->y, p->angle);
					if (p->type == FIRE) human->burn();
					
					// erase the particle
					if (p->type != FIRE && p->type != EXPLOSION && p->type != GAZ) {
						particles_.erase(p);
						--p;
					} else {
						p->damage = 0;
					}
				}
			} else if (p->adhoc == false ) {
				float cs = pgeMathCos(p->angle);
				float sn = pgeMathSin(p->angle);
				
				// laser, on vérifie tout le long du laser
				for (float i = -p->size/2; i < p->size/2; i += p->size/20) {

					float x = p->x + cs * i;
					float y = p->y + sn * i;

					if ( pgeMathSqrt(pow(x - human->getPos().x,2) + pow(y - ( human->getPos().y - 50),2) ) < 40) {

						// kill the perso
						human->inflictDamage(p->damage, x, y, p->angle);
						p->damage = 0;
					}
				}
			}
		}
	}

	for (std::vector<LaserParticle>::iterator l = lasers_.begin(); l != lasers_.end(); ++l) {

		/// Apply damages
		if (l->damage > 0 && l->adhoc == false) {

			if ( pgeMathSqrt(pow(l->x - human->getPos().x,2) + pow(l->y - ( human->getPos().y - 50),2) ) < 30) {
				// kill the perso
				human->inflictDamage(l->damage, l->x, l->y, l->angle);
			}
		}
	}
}

void Particles::draw() {
	
	for (std::vector<Particle>::iterator p = particles_.begin(); p != particles_.end(); ++p) {
		
		float alpha = (p->type != CARTOUCHE || p->type != GRENADE) ? p->life*20 : 255;
		if (p->type == FIRE) {
			float scale = 2.5f-((float)p->life/50);
			Sprite::setScale(scale,scale);
			int blue = (50-2*(50-p->life)) >= 0 ? 50-2*(50-p->life) : 0;
			Sprite::setColor(pgeGfxCreateColor(255,p->life*(255/50),blue*(255/50),255));
		} else if (p->type == LASER) {
			Sprite::setScale(p->size/100,p->life/3);
			alpha = p->life * 50;
		} else if (p->type == GAZ) {
			float scale = 3-((float)p->life/50);
			Sprite::setScale(scale,scale);
			Sprite::setColor(0xff00ff00);
		}
		p->sprite->draw2D(p->x,p->y, p->angle, alpha, false, false);
	}

	for (std::vector<LaserParticle>::iterator l = lasers_.begin(); l != lasers_.end(); ++l) {
		
		int lastVertice = l->vertices.size() - 1;
		float size = 10 + pgeMathSqrt(pow(l->x - l->vertices[lastVertice].x,2) + pow(l->y - l->vertices[lastVertice].y ,2) );
		float x = (l->x + l->vertices[lastVertice].x) / 2;
		float y = (l->y + l->vertices[lastVertice].y) / 2;
		float angle = l->angle;
		if ( ((float)l->vertices.size()/2) == ((int) (float)l->vertices.size()/2) ) angle *= -1;
		Sprite::setScale(size/100,1);
		l->sprite->draw2D(x,y,angle,255,false,false);
		
		for (int i = 0; i < l->vertices.size() - 1; i++) {
			size = 10 + pgeMathSqrt(pow(l->vertices[i].x - l->vertices[i+1].x,2) + pow(l->vertices[i].y - l->vertices[i+1].y ,2) );
			x = (l->vertices[i].x + l->vertices[i+1].x) / 2;
			y = (l->vertices[i].y + l->vertices[i+1].y) / 2;
			angle = l->angle;
			if ( ((float)i/2) != ((int) (float)i/2) ) angle *= -1;
			Sprite::setScale(size/100,1);
			l->sprite->draw2D(x,y,angle,255,false,false);
		}
		
	}
}
