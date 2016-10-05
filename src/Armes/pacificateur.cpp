#include "pacificateur.hpp"

Pacificateur::Pacificateur(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 8;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	cartoucheSprite_ = cartouche;
	
	sound_ = sound;
	
	chargeur_ = 10;
	munitions_ = 10;
	shootDelay_ = 20;
	shootDelayCurrent_ = 0;
	
	damage_ = 50;
	
	
	
	main1_.x = -20;  main1_.y = -13;
	main2_.x = 18;  main2_.y = -10;

	height_ = 0;
	width_ = 80;
	widthTir_ = 150;
}


bool Pacificateur::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	
	shootDelayCurrent_--;
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_ --;
		
		if (!pgeWavIsPlaying(sound_))
			pgeWavPlay(sound_);
			
		game_->vibrate(3,5);
		
		float X = x + cos(angle) * (width_ + 20);
		float Y = y + sin(angle) * (width_ + 20);

		//cartouche
		float cAngle = angle + pgeMathRandFloat(GU_PI/3, 2*GU_PI/3) * orientation;
		float cX = x + cos(angle) * width_/2 - sin(angle)*20*orientation;
		float cY = y + sin(angle) * width_/2 + cos(angle)*20*orientation;
		pSystem_->addParticle(cX,cY, pgeMathRandFloat(0,2*GU_PI), cos(cAngle), sin(cAngle)/2, 0,CARTOUCHE, cartoucheSprite_, 0,false);
		
		///
		pSystem_->addLaser(X,Y,pgeMathCos(angle), pgeMathSin(angle), damage_, angle, bulletSprite_, adhoc);
		///
		
		return true;	
	}
	return false;
}

void Pacificateur::update() {
	
}




