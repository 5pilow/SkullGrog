#include "gazeur.hpp"

Gazeur::Gazeur(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, pgeWav* sound)
: Arme(pSystem, sprite) {

	id_ = 9;
		
	fireSprite_ = bulletSprite;
		
	sound_ = sound;

	main1_.x = -2;  main1_.y = -10;
	main2_.x = 45;  main2_.y = -8;
	
	chargeur_ = 200;
	munitions_ = 200;
	surchauffe_ = 0;
	shootDelay_ = 4;
	shootDelayCurrent_ = 0;
	
	damageParticle_ = 10;
	width_ = 100;
	widthTir_ = 140;
	height_ = 10;
}

bool Gazeur::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	if (munitions_ > 0) {
		float c = pgeMathCos(angle), s = pgeMathSin(angle);
		
		// balle
		int damage = 0;
		damageParticle_--;
		if (damageParticle_ <= 0) {
			damageParticle_ = 10;
			damage = 10;
		}

		//sound
		if (!pgeWavIsPlaying(sound_) ){
			pgeWavPlay(sound_);
		}
		
		munitions_ --;
		
		angle += pgeMathRandFloat(-GU_PI/50,GU_PI/50);
		pSystem_->addParticle(x + c*widthTir_, y + s*widthTir_, angle, pgeMathCos(angle), pgeMathSin(angle), 0, GAZ, fireSprite_, damage, adhoc);
		
		return true;
	}
	return false;
}

void Gazeur::update() {
	
}


