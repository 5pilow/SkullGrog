#include "shotgun.hpp"

Shotgun::Shotgun(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 4;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	cartoucheSprite_ = cartouche;
	
	sound_ = sound;
	
	chargeur_ = 40;
	munitions_ = 40;
	surchauffe_ = 0;
	shootDelay_ = 30;
	shootDelayCurrent_ = 0;
	
	damage_ = 30;
	
	
	main1_.x = -20;  main1_.y = -10;
	main2_.x = 30;  main2_.y = -10;

	width_ = 90;
	widthTir_ = 160;
	height_ = 0;
}


bool Shotgun::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_ -= 5;
		
		if (pgeWavIsPlaying(sound_)) {
			pgeWavStop(sound_);
		}
		pgeWavPlay(sound_);
			
		game_->vibrate(3,5);
		
		float X = x + cos(angle) * widthTir_;
		float Y = y + sin(angle) * widthTir_;
		
		// explosion
		pSystem_->addParticle(X, Y, angle, 0,0, 0, EXPLOSION, explosion_[ (int) pgeMathRandInt(0,(int) explosion_.size() ) ], 0, false);
		
		
		//cartouche
		float cAngle = pgeMathRandFloat(GU_PI/4, 3*(GU_PI/4));
		float cX = x + cos(angle) * (width_ + 10);
		float cY = y + sin(angle) * (width_ + 10);
		pSystem_->addParticle(cX,cY, pgeMathRandFloat(0,2*GU_PI), pgeMathCos(cAngle), pgeMathSin(cAngle), 0,CARTOUCHE, cartoucheSprite_, 0, false);
		
		
		for (int i = 0; i < 5; i++) {
			// balle
			float a = angle + pgeMathRandFloat(-GU_PI/10,GU_PI/10);
			pSystem_->addParticle(X, Y, a, pgeMathCos(a), pgeMathSin(a), 0,BULLET, bulletSprite_, damage_, adhoc);
		}
		

		
		
		return true;	
	}
	return false;
}

void Shotgun::update() {
	shootDelayCurrent_--;
}




