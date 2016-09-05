#include "lanceFlammes.hpp"

LanceFlammes::LanceFlammes(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, pgeWav* sound)
: Arme(pSystem, sprite) {

	id_ = 7;
		
	fireSprite_ = bulletSprite;
		
	sound_ = sound;

	main1_.x = -4;  main1_.y = -10;
	main2_.x = 42;  main2_.y = -10;
	
	chargeur_ = 150;
	munitions_ = 150;
	surchauffe_ = 0;
	shootDelay_ = 4;
	shootDelayCurrent_ = 0;
	
	damageParticle_ = 15;
	width_ = 100;
	widthTir_ = 140;
	height_ = 10;
}

bool LanceFlammes::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	if (munitions_ > 0) {
		
		float c = pgeMathCos(angle), s = pgeMathSin(angle);
		
		// balle
		damageParticle_--;
		int damage = 0;
		if (damageParticle_ <= 0) {
			damage = 3;
			damageParticle_ = 10;
		}

		//sound
		if (!pgeWavIsPlaying(sound_) ){
			pgeWavPlay(sound_);
		}
		
		munitions_--;
		
		angle += pgeMathRandFloat(-GU_PI/50,GU_PI/50);
		pSystem_->addParticle(x + c*widthTir_, y + s*widthTir_, angle, pgeMathCos(angle), pgeMathSin(angle), 0, FIRE, fireSprite_, damage, adhoc);

		return true;
	}
	return false;
}

void LanceFlammes::update() {
	
}


