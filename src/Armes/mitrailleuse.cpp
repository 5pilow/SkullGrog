#include "mitrailleuse.hpp"

Mitrailleuse::Mitrailleuse(Particles* pSystem, Sprite* sprite, Sprite* bulletSprite, std::vector<Sprite*> exp, Sprite* cartouche, pgeWav* sound) 
: Arme(pSystem, sprite) {

	id_ = 1;
	
	bulletSprite_ = bulletSprite;
	explosion_ = exp;
	cartoucheSprite_ = cartouche;
	
	sound_ = sound;
	
	chargeur_ = 60;
	munitions_ = 60;
	surchauffe_ = 0;
	shootDelay_ = 25;
	shootDelayCurrent_ = 0;
	
	damage_ = 30;
	
	
	main1_.x = -10;  main1_.y = -10;
	main2_.x = 36;  main2_.y = -12;

	width_ = 80;
	widthTir_ = 140;
	height_ = 0;
}


bool Mitrailleuse::kill(float x, float y, float angle, int orientation, bool adhoc) {
	
	
	if (shootDelayCurrent_ <= 0 && munitions_ > 0) {
		shootDelayCurrent_ = shootDelay_;
		
		munitions_ -= 6;
		
		pgeWavPlay(sound_);
			
		game_->vibrate(3,5);
		
		float X = x + cos(angle) * widthTir_;
		float Y = y + sin(angle) * widthTir_;
		
		// explosion
		pSystem_->addParticle(X, Y, angle, 0,0, 0, EXPLOSION, explosion_[ (int) pgeMathRandInt(0,(int) explosion_.size() ) ], 0,false);
		
		
		//cartouche
		float cAngle = pgeMathRandFloat(GU_PI/4, 3*(GU_PI/4));
		float cX = x + cos(angle) * (width_ + 10);
		float cY = y + sin(angle) * (width_ + 10);
		pSystem_->addParticle(cX,cY, pgeMathRandFloat(0,2*GU_PI), pgeMathCos(cAngle), -pgeMathSin(cAngle), 0,CARTOUCHE, cartoucheSprite_, 0,false);
		
		
		angle -= 2*GU_PI/10;
		for (int i = 0; i < 3; i++) {
			// balle
			angle += GU_PI/10;
			angle -= GU_PI/40;
			pSystem_->addParticle(X, Y, angle, pgeMathCos(angle), pgeMathSin(angle), 0,BULLET, bulletSprite_, damage_, adhoc);
			angle += GU_PI/40;
			pSystem_->addParticle(X, Y, angle, pgeMathCos(angle), pgeMathSin(angle), 0,BULLET, bulletSprite_, damage_, adhoc);
		}
		

		
		
		return true;	
	}
	return false;
}

void Mitrailleuse::update() {

	shootDelayCurrent_--;
	
}




