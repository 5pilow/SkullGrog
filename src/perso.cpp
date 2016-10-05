#include "perso.hpp"

#define DEBUG 0

#define cos(x) pgeMathCos(x)
#define sin(x) pgeMathSin(x)

Perso::Perso(float x, float y, std::vector<Sprite*>& sprites, Particles* pSystem, Map* map) {
	
	pos_.x = x;
	pos_.y = y;
	
	angleViseur_ = 0;
	orientation_ = 1;
	jump_ = 0;
	numJump_ = 0;
	walkFrame_ = 0;
	breathFrame_ = 0;
	weaponRecul_ = 0;
	
	pSystem_ = pSystem;
	map_ = map;
	
	burning_ = 0;
	
	headSprite_ = sprites[0];
	bodySprite_ = sprites[1];
	footSprite_ = sprites[2];
	main1Sprite_ = sprites[3];
	main2Sprite_ = sprites[4];
	bloodSprite_ = sprites[5];
	bloodSprite2_ = sprites[6];
	
}

Perso::~Perso() {
	
}

void Perso::reset() {

	angleViseur_ = 0;
	orientation_ = 1;
}


void Perso::move(float dx, float dy) {
	
	if (dx != 0) {
		// move x
		pos_.x += dx;
		
		// collide x
		while (map_->collideX(pos_.x + Utils::normalize(dx)*30, pos_.y))
			pos_.x -= dx;
	}
	
	if (dy != 0) {
		// move y
		pos_.y += dy;

		if (dy > 0) 
			jump_ = -1;
		
		// collide y
		
		float newY = map_->collideY(pos_.x, pos_.y, dy);
		if (newY != pos_.y) {
			pos_.y = newY;
			jump_ = 0;
			if (dy > 0) {
				numJump_ = 0;
			}
		}
	}

}

void Perso::update() {
	
	float dy = 0;
	
	/// Jump & Gravity
	if (jump_ > 0) {
		// jump
		dy = -2.5f*jump_;
		jump_ -= 0.4f;
		if (jump_ <= 0) {
			jump_ = 0;
		}
	} else {
		// fall
		dy = 10;
	}
	
	move(0, dy);


	/// Breathing
	breathFrame_ ++;
	
	if (weaponRecul_ > 0) {
		weaponRecul_ -= 1.5f;
	}
	
	/// Burning ^^
	if (burning_ > 0) {

		burning_--;
		
		inflictDamage(1,pos_.x, pos_.y - 55,0);
		pSystem_->addParticle(pos_.x + pgeMathRandFloat(-20,20), - 55 + pos_.y + pgeMathRandFloat(-20,20),  0, 0,0, 0.1f, FIRE, NULL,0, false);
	}

	
}

int Perso::getLife() {
	
	return life_;
}

int Perso::getOrientation() {
	
	return orientation_;
}

PersoTeam Perso::getTeam() {

	return team_;
}
float Perso::getWalkFrame() {

	return walkFrame_;
}


void Perso::setPos(float x, float y) {
	
	pos_.x = x;
	pos_.y = y;
}

void Perso::setLife(int life) {

	life_ = life;
}

void Perso::setAngleViseur(float angle) {
	
	angleViseur_ = angle;
	if (angleViseur_ > GU_PI/2 && angleViseur_ < 3*GU_PI/2) {
		orientation_ = -1;
	} else {
		orientation_ = 1;
	}
}
void Perso::setWalkFrame(float walkFrame) {

	walkFrame_ = walkFrame;
}

void Perso::inflictDamage(float damage, float x, float y, float angle) {}
	

void Perso::burn() {
	
	burning_ = 100;
}
	
ScePspFVector2 Perso::getPos() {
	
	return pos_;
}
ScePspFVector2* Perso::getPosPtr() {
	
	return &pos_;
}
float Perso::getAngleViseur() {
	
	return angleViseur_;
}



void Perso::draw() {
	
	
}


void Perso::drawBody() {
	
	bool reverse = (orientation_ == 1) ? false : true;
	
	// Tête
	float y = pos_.y - 95 + cos(breathFrame_/10)*5;
	headSprite_->draw2D(pos_.x,y, angleViseur_, 255, reverse, false);
	
	// Corps
	y = pos_.y - 50 + cos(breathFrame_/10 + 0.6)*5;
	float x = pos_.x - orientation_*15;
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
	footSprite_->draw2D(x,y, ag, 255, false, reverse);
	a += GU_PI;
	sa = sin(a), ca = cos(a);
	y = sol + 15*sa; y = (y > sol) ? sol : y;
	x = pos_.x + 20*ca;
	ag = (-15*((sa > 0) ? 0 : sa))/15 * orientation_;
	if (incPied) ag = GU_PI/4 * orientation_ ;
	footSprite_->draw2D(x,y, ag, 255, false, reverse);
}
