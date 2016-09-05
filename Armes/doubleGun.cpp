#include "doubleGun.hpp"

#define cos(x) pgeMathCos(x)
#define sin(x) pgeMathSin(x)

DoubleGun::DoubleGun(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 2;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	cartoucheSprite_ = cartouche;
	
	sound_ = sound;
	
	chargeur_ = 32;
	munitions_ = 32;
	surchauffe_ = 0;
	shootDelay_ = 4;
	shootDelayCurrent_ = 0;
	
	damage_ = 20;
	
	canon_ = 1;
	
	main1_.x = 11;  main1_.y = -5;
	main2_.x = 34;  main2_.y = -8;
	height_ = 0;
}


bool DoubleGun::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	
	shootDelayCurrent_--;
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_--;
		
		canon_ = canon_ == 1 ? -1 : 1;
		
		// bruit
		if (!pgeWavIsPlaying(sound_))
			pgeWavPlay(sound_);
			
		// vibration
		game_->vibrate(1,2);
		
		// explosion
		float X = x + cos(angle) * (width_ + 30);
		float Y = y + sin(angle) * (width_ + 30);
		pSystem_->addParticle(X, Y, angle, 0,0, 0,EXPLOSION, explosion_[ (int) pgeMathRandInt(0,(int) explosion_.size() ) ], 0,false);
		
		//cartouche
		float cAngle = angle + pgeMathRandFloat(GU_PI/3, 2*GU_PI/3) * orientation;
		float cX = x + cos(angle) * width_/2 - sin(angle)*20*orientation;
		float cY = y + sin(angle) * width_/2 + cos(angle)*20*orientation;
		pSystem_->addParticle(cX,cY, pgeMathRandFloat(0,2*GU_PI), pgeMathCos(cAngle), pgeMathSin(cAngle)/2, 0,CARTOUCHE, cartoucheSprite_, 0,false);
		
		// balles
		float bX = X - sin(angle)*10*canon_;
		float bY = Y + cos(angle)*10*canon_;
		angle += GU_PI/50*canon_;
		pSystem_->addParticle(bX, bY, angle, pgeMathCos(angle), pgeMathSin(angle), 0,BULLET, bulletSprite_, damage_, adhoc);
		
		return true;	
	}
	
	return false;
}

void DoubleGun::update() {
	
	
}





