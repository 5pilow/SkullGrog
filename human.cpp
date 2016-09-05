#include "human.hpp"
#include "perso.hpp"

Human::Human(float x, float y, std::vector<Sprite*>& sprites, Particles* pSystem, Map* map, Stats* stats, Adhoc* adhoc, bool isAdhoc, char* name) 
: Perso(x,y,sprites,pSystem,map) {
	
	team_ = HUMAN;
	life_ = 1000;
	speed_ = 5;
	
	currentArme_ = 0;
	
	stats_ = stats;

	sprites_ = sprites;
	
	arme_ = NULL;

	name_ = name;

	adhoc_ = adhoc;
	isAdhoc_ = isAdhoc;
}

Human::~Human() {}

void Human::reset(bool adhoc) {
	
	life_ = 1000;
	Perso::reset();	

	for (int i = 0; i < armes_.size(); i++)
		delete armes_[i];
	armes_.clear();
	
	stats_->setPersoWeapons(&armes_, adhoc);
	
	currentArme_ = 0;
	arme_ = armes_[0];
}

void Human::addArme(Arme* arme) {
	
	armes_.push_back(arme);	
}

void Human::setArme(Arme* arme) {
	
	arme_ = arme;
}
void Human::setArmeID(int id) {

	arme_ = armes_[id];
}

Arme* Human::getArme() {
	
	return arme_;	
}

std::vector<Sprite*>& Human::getSprites() {

	return sprites_;
}


void Human::inflictDamage(float damage,float x, float y, float angle) {

	if (!isAdhoc_) {
		life_ -= damage;
	}
		
	// tache de sang
	if (damage > 2)
		pSystem_->addParticleFond(pgeMathRandInt(0,2)==0?bloodSprite_:bloodSprite2_, x, y, pgeMathRandFloat(0,GU_PI*2), 50);

}	

void Human::addLife(int life) {
	
	life_ += life;
	if (life_ > 1000) life_ = 1000;	
}

void Human::addMunitions() {
		
	for (int i = 0; i < armes_.size(); i++) {
		armes_[i]->addMunitions();
	}
	
}



char* Human::getName() {
	
	return name_;
}

void Human::move() {
	
	float dx = 0;
	
	if ( (stats_->getControls() == 0 && pgeControlsHeld(PGE_CTRL_SQUARE) ) ||
		 (stats_->getControls() == 1 && pgeControlsAnalogX() < -80 ) ) {

		dx = -speed_;
		walkFrame_ -= 0.1;
		
	} else if ( (stats_->getControls() == 0 && pgeControlsHeld(PGE_CTRL_CIRCLE) ) ||
		 (stats_->getControls() == 1 && pgeControlsAnalogX() > 80) ) {

		dx = speed_;
		walkFrame_ += 0.1;
		
	} else {
		walkFrame_ = 0;
	}
	
	Perso::move(dx, 0);
	
		
	if (stats_->getControls() == 0) {
		if (pgeControlsAnalogMag() > 0.8) {
			angleViseur_ = pgeControlsAnalogAngle() - GU_PI/2;
			
			if (angleViseur_ > GU_PI/2 && angleViseur_ < 3*GU_PI/2) {
				orientation_ = -1;
			} else {
				orientation_ = 1;
			}
		}
	} else if (stats_->getControls() == 1) {
		if (pgeControlsPressed(PGE_CTRL_SQUARE)) {
			angleViseur_ = -GU_PI;
			orientation_ = -1;
		}
		if (pgeControlsPressed(PGE_CTRL_CIRCLE)) {
			angleViseur_ = 0;
			orientation_ = 1;
		}
	}
	
	// Jump	
	if ( pgeControlsPressed(PGE_CTRL_LTRIGGER) )  {
		if (numJump_ < 2) {
			numJump_ ++;
			jump_ = (numJump_ == 1) ? 6 : 7.5f;
		}
	}

	// Tir
	if ( (arme_->getTirTrue() && pgeControlsHeld(PGE_CTRL_RTRIGGER))  ||  (!arme_->getTirTrue() &&  pgeControlsPressed(PGE_CTRL_RTRIGGER)) ) {

		shoot();
	}
	
	// Changement d'arme
	if (pgeControlsPressed(PGE_CTRL_RIGHT)) {
		
		currentArme_++;
		if (currentArme_ > (int) armes_.size() - 1) 
			currentArme_ = 0;
		setArme(armes_[currentArme_]);
	}
	if (pgeControlsPressed(PGE_CTRL_LEFT)) {
		currentArme_--;
		if (currentArme_ < 0) 
			currentArme_ = armes_.size() - 1;
		setArme(armes_[currentArme_]);
	}
	

		
}

void Human::shoot() {
	
	float x = pos_.x - cos(angleViseur_)*weaponRecul_ + sin(angleViseur_)*arme_->getHeight()*orientation_;
	float y = pos_.y - 65 - sin(angleViseur_)*weaponRecul_ - cos(angleViseur_)*arme_->getHeight()*orientation_;

	float dx = cos(angleViseur_) * arme_->getWidthTir();
	float dy = sin(angleViseur_) * arme_->getWidthTir();

	bool collision = false;
	for (int i = 0; i < 5; i++) {
		x += dx/5;
		y += dy/5;
		if (map_->collideX(x,y)) {
			collision = true;
			break;
		}
	}
	if (collision == false)
		if (arme_->kill(x-dx, y-dy, angleViseur_, -orientation_, isAdhoc_)) {
			weaponRecul_ = 15;
			if (!isAdhoc_) {
				adhoc_->shoot((int)pos_.x, (int)pos_.y,arme_->getID(), angleViseur_);
			}
		}
}

void Human::updateHuman() {

	// update des armes du perso
	for (int i = 0; i < (int)armes_.size(); i++) {

		armes_[i]->update();
	}
}

void Human::draw() {
	
	if (orientation_ == 1) {
		drawBody();
		drawArme();
	} else if (orientation_ == -1) {
		drawArme();
		drawBody();
	}
	
}

void Human::drawArme() {

	
	bool orientation = (orientation_ == 1) ? false : true;
	
	float a = angleViseur_ + arme_->getAngle() * orientation_;
	float cosAngle = cos(a);
	float sinAngle = sin(a);
	
	// arme
	float x = pos_.x + cosAngle * (arme_->getWidth()/2 - weaponRecul_) + sinAngle*arme_->getHeight()*orientation_;
	float y = pos_.y - 60 + cos(breathFrame_/10)*4 + sinAngle * (arme_->getWidth()/2 - weaponRecul_) - cosAngle*arme_->getHeight()*orientation_;
	
	arme_->draw(x, y, a, 255, orientation);
	
	// main 1
	x = pos_.x + cosAngle * (arme_->getPosMain(1).x - weaponRecul_) + sinAngle*arme_->getPosMain(1).y*orientation_;
	y = pos_.y - 60 + cos(breathFrame_/10)*4 + sinAngle * (arme_->getPosMain(1).x- weaponRecul_) - cosAngle*arme_->getPosMain(1).y*orientation_;
	
	(orientation_ == 1 ? main1Sprite_ : main2Sprite_)->draw2D(x,y, a, 255, orientation, false);
	
	// main 2
	x = pos_.x + cosAngle * (arme_->getPosMain(2).x- weaponRecul_) + sinAngle*arme_->getPosMain(2).y*orientation_;
	y = pos_.y - 60 + cos(breathFrame_/10)*4 + sinAngle * (arme_->getPosMain(2).x- weaponRecul_) - cosAngle*arme_->getPosMain(2).y*orientation_;
	
	(orientation_ == 1 ? main2Sprite_ : main1Sprite_)->draw2D(x,y, a, 255, orientation, false);

}
