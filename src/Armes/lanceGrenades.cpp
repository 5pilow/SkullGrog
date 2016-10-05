#include "lanceGrenades.hpp"

#define cos(x) pgeMathCos(x)
#define sin(x) pgeMathSin(x)

LanceGrenades::LanceGrenades(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 5;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	
	sound_ = sound;
	
	chargeur_ = 12;
	munitions_ = 12;
	surchauffe_ = 0;
	shootDelay_ = 5;
	shootDelayCurrent_ = 0;
	
	damage_ = 0;

	widthTir_ = 110;
	width_ = 110;
	main1_.x = 31;  main1_.y = -5;
	main2_.x = 61;  main2_.y = -8;
	height_ = 0;

	tirTrue_ = false;
}


bool LanceGrenades::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_ --;
		
		// bruit
		pgeWavPlay(sound_);
			
		// vibration
		game_->vibrate(1,2);
		
		// explosion
		float X = x + cos(angle) * widthTir_;
		float Y = y + sin(angle) * widthTir_;
		pSystem_->addParticle(X, Y, angle, 0,0, 0,EXPLOSION, explosion_[ (int) pgeMathRandInt(0,(int) explosion_.size() ) ], 0,false);
		
		// balles
		angle += pgeMathRandFloat(-GU_PI/50,GU_PI/50);
		pSystem_->addParticle(X, Y, angle, pgeMathCos(angle), pgeMathSin(angle), 0,GRENADE, bulletSprite_, 0, adhoc);
		
		return true;	
	}
	
	return false;
}

void LanceGrenades::update() {

	if (shootDelayCurrent_ > 0)
		shootDelayCurrent_--;

}





