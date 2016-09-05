#include "zombie.hpp"
#include "perso.hpp"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define max(x,y) (x > y ? x : y)
#define abs pgeMathAbs



Zombie::Zombie(float x, float y, std::vector<Sprite*>& sprites, std::vector<Sprite*>& bouts, Particles* pSystem, Map* map, std::vector<pgeWav*>& cris)
: Perso(x,y,sprites,pSystem,map) {
	
	team_ = ZOMBIE;
	life_ = 100;
	speed_ = 1;

	finish_ = false;
	dead_ = false;
	
	target_.x = 0;
	target_.y = 0;
	
	searchPath_ = 0;
	dirX_ = 0;
	dirY_ = 0;

	bouts_ = bouts;

	attack_= false;
	attackFrame_ = 0;
	deadFrame_ = 0;

	screams_ = cris;
}


Zombie::~Zombie() {}

void Zombie::reset() {
	
	life_ = 100;	
}

void Zombie::setTarget(ScePspFVector2 target) {
	
	target_ = target;
}

void Zombie::setSpeed(float speed) {
	
	speed_ = speed;
}


void Zombie::move() {

	if (!dead_) {
	
		if (attack_)
			attackFrame_ ++;

		if (jump_ == 0)
			nextDirX_ = 0;

		int jumpHeight = 0;
		
		searchPath_--;
		if (searchPath_ <= 0) {
			searchPath_ = pgeMathRandInt(15,25);
			chemin_.clear();
			Tile start = map_->getTile(pos_.x, pos_.y);
			Tile end = map_->getTile(target_.x, target_.y);
			
			if (start.x != end.x || start.y != end.y) {
				chemin_ = map_->findPath(start, end);
				attack_ = false;
			} else {
				attack_ = true;
			}

			if (chemin_.size() > 1) {
				dirX_ = chemin_[1].x - start.x;
				dirY_ = chemin_[1].y - start.y;
			}

			if (dirX_ == 0 && chemin_.size() >= 3) {
			
				nextDirX_ = 0;
				Tile tile = chemin_[1];
				
				for (int i = 2; i < chemin_.size(); i++) {
					jumpHeight++;
					short dirX = chemin_[i].x - start.x;
					if (dirX != 0) {
						nextDirX_ = dirX;
						break;
					}
				}
			}

			if (dirY_ < 0) { // On verifie si le saut est nécessaire
				for (int i = 1; i < 4; i++) {
					if (map_->getMapTile(start.x,start.y-i) == 9) {
						dirY_ = 0;
						dirX_ = nextDirX_;
						break;
					}
				}
			}
		}
		
		float dx = 0;
		
		if (dirX_ > 0) {
			// move right
			dx = speed_ ;
			walkFrame_ += speed_/40 ;
			orientation_ = 1;
			angleViseur_ = 0;
			
		} else if (dirX_ < 0) {
			// move left
			dx = -speed_ ;
			walkFrame_ -= speed_/40;
			orientation_ = -1;
			angleViseur_ = GU_PI;
			
		} else {
			walkFrame_ = 0;
		}
		
		Perso::move(dx, 0);
		
		if (nextDirX_ != 0 && pgeMathAbs(jump_) < 4) {
			Perso::move(nextDirX_*4,0);
			if (nextDirX_ < 0) {
				walkFrame_ -= speed_/40;
				orientation_ = -1;
				angleViseur_ = GU_PI;
			} else {
				walkFrame_ += speed_/40 ;
				orientation_ = 1;
				angleViseur_ = 0;
			}
		}
		
		if (dirY_ < 0 && dirX_ == 0 && numJump_ < 1) {

			// jump
			numJump_ ++;
			if (jumpHeight == 1)
				jump_ += 5.5f;
			else if (jumpHeight == 2)
				jump_ += 7;
			else if (jumpHeight == 3)
				jump_ += 8;
			else if (jumpHeight == 4)
				jump_ += 9;
			else if (jumpHeight == 5)
				jump_ += 10;
		}
		
	} else if (!finish_) {

		deadFrame_++;
		
		if (deadFrame_ > 50) {
			finish_ = true;
			return;
		}
		
	}
	
}

bool Zombie::isFinish() {

	return finish_;
}

bool Zombie::isDead() {

	return dead_;
}

void Zombie::inflictDamage(float damage, float x, float y,float angle) {

	
	if (!dead_) {
		
		life_ -= damage;
		
		// tache de sang
		if (damage > 1) {
			pSystem_->addParticleFond(pgeMathRandInt(0,2)==0?bloodSprite_:bloodSprite2_, x, y, pgeMathRandFloat(0,GU_PI*2), 50);
		}
		
		if (damage > 1) {
			pSystem_->addParticle(x,y, pgeMathRandFloat(0,2*GU_PI), cos(angle)/2, sin(angle)/2, pgeMathRandFloat(1,3), BLOOD, pgeMathRandInt(0,2)==0?bloodSprite_:bloodSprite2_, 0, false);
			pSystem_->addParticle(x,y, pgeMathRandFloat(0,2*GU_PI), cos(angle+GU_PI)/2, sin(angle+GU_PI)/2, pgeMathRandFloat(1,3), BLOOD, pgeMathRandInt(0,2)==0?bloodSprite_:bloodSprite2_, 0, false);
		}
		
		if (life_ <= 0) {

			dead_ = true;
			
			pgeWavPlay(screams_[pgeMathRandInt(0,screams_.size())]);
			
			if (damage >= 100) { // Explosion

				finish_ = true;
			
				for (int i = 0; i < bouts_.size(); i++) {
					float angle = pgeMathRandFloat(0,2*GU_PI);
					pSystem_->addParticle(x,y, 0, cos(angle), sin(angle), 1, BOUT, bouts_[i], 0, false);
				}

				
				// un peu de sang en supplément
				//~ for (int i = 0; i < 2; i++) {
					//~ float angle = pgeMathRandFloat(0,2*GU_PI); 
					//~ pSystem_->addParticle(x,y, angle, cos(angle)/2, sin(angle)/2, pgeMathRandFloat(1,3), BLOOD, bloodSprite_, 0);
				//~ }
			}
		}
	}
}


void Zombie::draw() {

	int black = 255-deadFrame_*2.55f*2;

	if (!dead_)
		drawBody();
	else {

		bool reverse = (orientation_ == 1) ? false : true;
	
		// Tête
		float y = pos_.y - 95 + cos(breathFrame_/10)*5;
		Sprite::setColor(pgeGfxCreateColor(black,black,black,black));
		headSprite_->draw2D(pos_.x,y, angleViseur_, 255, reverse, false);
		
		// Corps
		y = pos_.y - 50 + cos(breathFrame_/10 + 0.6)*5;
		float x = pos_.x - orientation_*15;
		Sprite::setColor(pgeGfxCreateColor(black,black,black,black));
		bodySprite_->draw2D(x,y, (float)orientation_/7.5f, 255, false,reverse);
		
		// Pieds, avec animation
		bool incPied = (jump_ != 0);
		const float sol = pos_.y - 6;
		float a = walkFrame_ * 2.5f;
		float sa = sin(a), ca = cos(a);
		y = sol + 15*sa; y = (y > sol) ? sol : y;
		x = pos_.x + 20*ca;
		float ag = (-15*((sa > 0) ? 0 : sa))/15 * orientation_ ;
		if (incPied) ag = GU_PI/6 * orientation_;
		Sprite::setColor(pgeGfxCreateColor(black,black,black,black));
		footSprite_->draw2D(x,y, ag, 255, false, reverse);
		a += GU_PI;
		sa = sin(a), ca = cos(a);
		y = sol + 15*sa; y = (y > sol) ? sol : y;
		x = pos_.x + 20*ca;
		ag = (-15*((sa > 0) ? 0 : sa))/15 * orientation_;
		if (incPied) ag = GU_PI/4 * orientation_ ;
		Sprite::setColor(pgeGfxCreateColor(black,black,black,black));
		footSprite_->draw2D(x,y, ag, 255, false, reverse);
		
	}

	bool orientation = (orientation_ == 1) ? false : true;

	float atkX = 0,atkY = 0;
	float angle = 0;
	if( attack_) {
		atkX = sin(attackFrame_/2)*20*orientation_;
		//~ atkY = sin(attackFrame_/2)*40;
		angle = sin(attackFrame_/2)*orientation_;
	}

	// main 1
	float x = pos_.x + 30*orientation_ + atkX;
	float y = pos_.y - 65 + cos(breathFrame_/10)*5 + atkY;
	if (dead_) Sprite::setColor(pgeGfxCreateColor(black,black,black,black)) ;
	(orientation_ == 1 ? main2Sprite_ : main1Sprite_)->draw2D(x,y, angle, 255, false, orientation);
	
	// main 2
	x = pos_.x + 30*orientation_ + atkX;
	y = pos_.y - 65 + sin(breathFrame_/10)*5 + atkY;
	if (dead_) Sprite::setColor(pgeGfxCreateColor(black,black,black,black)) ;
	(orientation_ == 1 ? main1Sprite_ : main2Sprite_)->draw2D(x,y, angle, 255, false, orientation);
	
}


