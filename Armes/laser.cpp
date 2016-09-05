#include "laser.hpp"

#define cos(x) cos(x)
#define sin(x) sin(x)

Laser::Laser(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 6;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	cartoucheSprite_ = cartouche;
	
	sound_ = sound;
	
	chargeur_ = 10;
	munitions_ = 10;
	surchauffe_ = 0;
	shootDelay_ = 20;
	shootDelayCurrent_ = 0;
	
	damage_ = 100;
	
	main1_.x = -20;  main1_.y = -5;
	main2_.x = 26;  main2_.y = -14;
	
	width_ = 50;
	widthTir_ = 160;
	height_ = 0;
}


bool Laser::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_--;

		// bruit
		pgeWavPlay(sound_);
			
		// vibration
		game_->vibrate(4,6);
		
		float X = x + cos(angle) * (width_ + 30);
		float Y = y + sin(angle) * (width_ + 30);
		
		//cartouche
		float cAngle = angle + pgeMathRandFloat(GU_PI/3, 2*GU_PI/3) * orientation;
		float cX = x + cos(angle) * width_/2 - sin(angle)*20*orientation;
		float cY = y + sin(angle) * width_/2 + cos(angle)*20*orientation;
		pSystem_->addParticle(cX,cY, pgeMathRandFloat(0,2*GU_PI), cos(cAngle), sin(cAngle)/2, 0, CARTOUCHE, cartoucheSprite_, 0,false);
		
		// on calcule la taille du laser
		float lx = X, ly = Y;
		float size = 0;
	    while (!game_->getMap()->collideX(lx, ly) && size < 500) {
			size += 10;
			lx += cos(angle) * 10;
			ly += sin(angle) * 10;
		}
		if (size > 0) {
			float cs = cos(angle), sn = sin(angle);
			pSystem_->addParticle(X+cs*size/2, Y+sn*size/2, angle, cs,sn, size,LASER, bulletSprite_, damage_, adhoc);
		}
		return true;	
	}
	
	return false;
}

void Laser::update() {
		
	shootDelayCurrent_--;
}





