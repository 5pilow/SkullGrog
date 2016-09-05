#include "magnum.hpp"

#define cos(x) pgeMathCos(x)
#define sin(x) pgeMathSin(x)

Magnum::Magnum(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 3;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	cartoucheSprite_ = cartouche;
	
	sound_ = sound;
	
	
	chargeur_ = 40;
	munitions_ = 10;
	surchauffe_ = 0;
	shootDelay_ = 5;
	shootDelayCurrent_ = 0;
	
	damage_ = 50;

	widthTir_ = 130;
	width_ = 130;
	main1_.x = 48;  main1_.y = 0;
	main2_.x = 48;  main2_.y = -7;
	height_ = 0;

	tirTrue_ = false;
}


bool Magnum::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_--;
		
		// bruit
		if (!pgeWavIsPlaying(sound_))
			pgeWavPlay(sound_);
			
		// vibration
		game_->vibrate(1,2);
		
		// explosion
		float X = x + cos(angle) * widthTir_;
		float Y = y + sin(angle) * widthTir_;
		pSystem_->addParticle(X, Y, angle, 0,0, 0,EXPLOSION, explosion_[ (int) pgeMathRandInt(0,(int) explosion_.size() ) ], 0,false);
		
		//cartouche
		float cAngle = angle + pgeMathRandFloat(GU_PI/3, 2*GU_PI/3) * orientation;
		float cX = x + cos(angle) * width_/2 - sin(angle)*20*orientation;
		float cY = y + sin(angle) * width_/2 + cos(angle)*20*orientation;
		pSystem_->addParticle(cX,cY, pgeMathRandFloat(0,2*GU_PI), pgeMathCos(cAngle), pgeMathSin(cAngle)/2,0, CARTOUCHE, cartoucheSprite_, 0,false);
		
		// balles
		angle += pgeMathRandFloat(-GU_PI/50,GU_PI/50);
		pSystem_->addParticle(X, Y, angle, pgeMathCos(angle)*2, pgeMathSin(angle)*2, 0,BULLET, bulletSprite_, damage_, adhoc);
		
		return true;	
	}
	
	return false;
}

void Magnum::update() {

	if (shootDelayCurrent_ > 0)
		shootDelayCurrent_--;

}





