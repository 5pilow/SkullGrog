#include "arme.hpp"

#include "sprite.hpp"
#include "game.hpp"
#include "particles.hpp"

Arme::Arme(Particles* pSystem, Sprite* sprite) {
	
	pSystem_ = pSystem;
	sprite_ = sprite;
	
	width_ = sprite_->getWidth();
	height_ = sprite_->getHeight();
	
	widthTir_ = width_; // par défaut
	angle_ = 0;

	tirTrue_ = true;
	
	chargeur_= 0;
	munitions_= 0;
	surchauffe_= 0;
	shootDelay_= 0;
	shootDelayCurrent_= 0;
}

Arme::~Arme() {}

int Arme::getWidth() {
	
	return width_;
}

int Arme::getWidthTir() {
	
	return widthTir_;
}

int Arme::getID() {

	return id_;
}

int Arme::getHeight() {
	
	return height_;
}

float Arme::getAngle() {
	
	return angle_;
}

bool Arme::getTirTrue() {

	return tirTrue_;
}

Sprite* Arme::getSprite() {
	
	return sprite_;
}

int Arme::getMunitions() {
	
	return munitions_;	
}

int Arme::getChargeur() {
	
	return chargeur_;	
}


void Arme::noDelay() {

	shootDelayCurrent_ = 0;
	munitions_ = chargeur_*10;
}

void Arme::setGame(Game* game) {
	
	game_ = game;
}

void Arme::addMunitions() {
	
	munitions_ += chargeur_;	
}


void Arme::draw(float x, float y, float angle, unsigned char alpha, bool reverse) {
	
	sprite_->draw2D(x,y,angle, alpha, reverse, false);
}

ScePspFVector2 Arme::getPosMain(int main) {
	
	if (main == 1)
		return main1_;
	if (main == 2)
		return main2_;
}

bool Arme::kill(float x , float y, float angle, int orientation, bool adhoc) {

	return false;
}

void Arme::update() {}


