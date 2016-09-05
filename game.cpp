#include "game.hpp"

#define MAX_ZOMBIES 25
#define MAX_BONUS 400

std::vector<AdhocDataPacket> Game::adhocMsg;

Game::Game(Map* map, intraFont* font, Stats* stats, std::vector<Sprite*> icons, std::vector<Sprite*> bonus, Adhoc* adhoc, pgeWav* heartBreath) {
		
	map_ = map;
	stats_ = stats;
	
	camera_.x = 0;
	camera_.y = -136; 
	
	cameraDest_ = camera_;
	
	vibrationTime_ = 0;
	vibrationForce_ = 3;

	tirTrue_ = false;
	tirTrueTime_ = 0;
	
	pause_ = true;
	time_ = 0;
	timer_ = pgeTimerCreate();
	kills_ = 0;
	score_ = 0;
	multiplicateur_ = 1;
	multiplicateurTime_ = 0;

	isCompteARebourds_ = false;
	timeCompteARebourds_ = 0;

	addZombie_ = 0;
	numZombies_ = 0;

	finish_ = false;

	font_ = font;
	
	icons_ = icons;
	bonusSprite_ = bonus;
	
	hero_ = NULL;
	human_ = NULL;

	adhoc_ = adhoc;
	isAdhoc_ = false;
	server_ = false;

	heartBreath_ = heartBreath;

	debug_ = 0;
}


Map* Game::getMap() {

	return map_;
}

int Game::getScore() {
	return score_;
}

void Game::initNewGame(bool adhoc, bool server) {

	setPause(false);

	map_->reset();

	pSystem_->reset();

	for (int i = zombies_.size()-1; i >= 0; i--) {
		delete zombies_[i];
		zombies_.erase(zombies_.begin() + i);
	}

	kills_ = 0;
	score_ = 0;
	scoreDisplay_ = 0;
	multiplicateur_ = 1;
	addZombie_ = 0;
	addZombie_ = 0;
	numZombies_ = 0;
	time_ = 0;

	finish_ = false;

	isAdhoc_ = adhoc;
	server_ = server;

	delete hero_;

	if (isAdhoc_) {

		// erase old packets
		adhocMsg.clear();

		// you
		int x,y;
		if (server_) {
			x = 200; y = 150;
		} else {
			x =1300; y = 800;
		}
		hero_ = new Human(x,y,human_->getSprites(), pSystem_, map_,  stats_, adhoc_, false, "");
		hero_->reset(false);
		
		// friend
		if (server_) {
			x =1300; y = 800;
		} else {
			x = 200; y = 150;
		}
		char* name = server_ ? adhoc::server::getClientList().at(0)->hellodata : adhoc::client::getTargetPSP()->hellodata;
		adhocPlayer_ = new Human(x,y,human_->getSprites(), pSystem_, map_, stats_, adhoc_, true, name);
		adhocPlayer_->reset(true);
		
	} else {
		
		hero_ = new Human(200,150,human_->getSprites(), pSystem_, map_, stats_, adhoc_, false, "");
		hero_->reset(false);
	}

	isCompteARebourds_ = true;
	timeCompteARebourds_ = 4*50;

	timer_ = pgeTimerCreate();
}


void Game::endGame() {

	stats_->kills(kills_);
	stats_->time(time_);
	stats_->games(1);
	bool unlocked = stats_->checkUnlocks(kills_,score_,time_);
	//~ stats_->saveLocalScore(score_);
	stats_->save();
}


void Game::setPause(bool state) {
	
	pause_ = state;
}

bool Game::getPause() {
	
	return pause_;
}

Human* Game::getHero() {

	if (hero_)
		return hero_;
	return NULL;
}

int Game::getHeroLife() {

	return hero_->getLife();
}

ScePspFVector2 Game::getHeroPos() {

	return hero_->getPos();
}

float Game::getHeroAngle() {

	return hero_->getAngleViseur();
}

int Game::getNumZombies() {
	
	return zombies_.size();
}
int Game::getNumZombiesKilled() {
	
	return numZombies_;
}

void Game::setAdhocPlayerValues(int life, int x,int y,  int arme,float angle, float walkFrame) {
	adhocPlayer_->setLife(life);
	adhocPlayer_->setPos((float)x, (float)y);
	adhocPlayer_->setAngleViseur(angle);
	adhocPlayer_->setWalkFrame(walkFrame);
	adhocPlayer_->setArmeID(arme);
}

void Game::adhocPlayerShoot(int x, int y, int arme, float angle) {
	adhocPlayer_->setPos((float)x, (float)y);
	adhocPlayer_->setAngleViseur(angle);
	adhocPlayer_->setArmeID(arme);
	adhocPlayer_->getArme()->noDelay();
	adhocPlayer_->shoot();
}



void Game::addZombie(Zombie* zombie) {

	if (zombies_.size() < MAX_ZOMBIES) {

		zombies_.push_back(zombie);
		numZombies_++;

		addZombie_ = 100 - (int)((float)getNumZombiesKilled() / 40);
		if (addZombie_ < 0) addZombie_ = 0;
	}
}

void Game::addZombie(int x,int y,float speed) {
	
	Zombie* z = new Zombie(*zombie_);
	z->setPos(x,y);
	z->setSpeed(speed);
	addZombie(z);
}

bool Game::addZombie() {
	return addZombie_ <= 0;
}

void Game::setHuman(Human* human) {

	human_ = human;
}
void Game::setZombie(Zombie* zombie) {

	zombie_ = zombie;
}

void Game::setParticleSystem(Particles* pSystem) {
		
	pSystem_ = pSystem;
}

void Game::removeZombie(int index) {

	Zombie* zombie = zombies_[index];
	zombies_.erase(zombies_.begin() + index);
	delete zombie;
}

ScePspFVector2 Game::getCamera() {
	
	ScePspFVector2 cam = {camera_.x, camera_.y};
	return cam;
}

bool Game::getFinish() {

	return finish_;
}

bool Game::isAdhoc() {
	
	return isAdhoc_;
}

void Game::increaseScore(int points) {

	score_ += points * multiplicateur_;
	multiplicateurTime_ = 60 * 4; // 4 secondes
	multiplicateurKills_++;
	if (multiplicateurKills_ > pgeMathPow(multiplicateur_,3)) {
		if (multiplicateur_ < 5)
			multiplicateur_ ++;
		multiplicateurKills_ = 0;
	}
}

void Game::addBonus() {

	if (bonus_.size() < MAX_BONUS) {
		// position
		float x, y;
		do {
			x = pgeMathRandInt(1,30)*50+25;
			y = pgeMathRandInt(1,18)*50+25;
		} while ( map_->collideX(x,y) );
		
		//type
		int type = pgeMathRandInt(0,4);
		
		if (isAdhoc_ && server_) {
			if (type == BOMBE) type = pgeMathRandInt(0,3);
			adhoc_->addBonus(x,y,type);
		}
		addBonus(x,y,type);
	}
}

void Game::addBonus(int x, int y, int type) {

	Bonus bonus = {x,y,type,0};
	bonus_.push_back(bonus);
}

void Game::bonusCollide() {
	
	for (int i = 0; i < bonus_.size(); i++) {
		
		//update bonus
		bonus_[i].frame ++;

		if (!isAdhoc_) {
			if (pgeMathSqrt(pow(bonus_[i].x - hero_->getPos().x,2) + pow(bonus_[i].y - (hero_->getPos().y - 50),2) ) < 50) {
				
				if ( bonus_[i].type == VIE ) {
					hero_->addLife(250);	
				} else if ( bonus_[i].type == MUNITIONS ) {
					hero_->addMunitions();
				} else if ( bonus_[i].type == BOMBE ) {
					for (int k = 0; k < zombies_.size(); k++) {
						zombies_[k]->inflictDamage(100, zombies_[k]->getPos().x, zombies_[k]->getPos().y - 50, 0); 
					}
					// après une bombe, on met un petit délai pour eviter un lag
					addZombie_ = 120;
				} else if ( bonus_[i].type == TIR_TRUE ) {
					tirTrue_ = true;
					tirTrueTime_ = 5*60;
				}
				bonus_.erase(bonus_.begin() + i);
				i--;
				continue;
			}
		} else {
			if (pgeMathSqrt(pow(bonus_[i].x - hero_->getPos().x,2) + pow(bonus_[i].y - (hero_->getPos().y - 50),2) ) < 50) {
				
				if ( bonus_[i].type == VIE ) {
					hero_->addLife(250);	
				} else if ( bonus_[i].type == MUNITIONS ) {
					hero_->addMunitions();
				} else if ( bonus_[i].type == TIR_TRUE ) {
					tirTrue_ = true;
					tirTrueTime_ = 5*60;
				}
				bonus_.erase(bonus_.begin() + i);
				i--;
				continue;
			}
			
			if (pgeMathSqrt(pow(bonus_[i].x - adhocPlayer_->getPos().x,2) + pow(bonus_[i].y - (adhocPlayer_->getPos().y - 50),2) ) < 50) {
			
				bonus_.erase(bonus_.begin() + i);
				i--;
			}
		}
	}
}


void Game::update() {

	if (isCompteARebourds_) {
		timeCompteARebourds_--;
		if (timeCompteARebourds_ <= 0) {
			isCompteARebourds_ = false;
		}
	}

	/// Time
	if (!pause_) {
		time_ += pgeTimerGetDeltaTime(timer_);
	}
	pgeTimerUpdate(timer_);
	
	if (pause_) return;
	
	time_ += pgeTimerGetDeltaTime(timer_);
	pgeTimerUpdate(timer_);

	if (tirTrue_) {
		tirTrueTime_--;
		if (tirTrueTime_ <= 0) {
			tirTrue_ = false;
		}
	}

	
	if (!isCompteARebourds_ && (!isAdhoc_ ) ) {
		addZombie_--;
	}
	
	
	multiplicateurTime_ --;
	if (multiplicateurTime_ <= 0) {
		multiplicateurKills_ = 0;
		multiplicateur_ = 1;
	}

	if (scoreDisplay_ < score_) {
		scoreDisplay_ += (score_ - scoreDisplay_)/10;
		if (score_ - scoreDisplay_ < 1) {
			scoreDisplay_ = score_;
		} 
	}
	
	
	
	if (!isAdhoc_ || server_) {
		addBonus_--;
		if (addBonus_ <= 0) {
			addBonus_ = 60 * 10; // 10s
			addBonus();
		}
	}

	bonusCollide();
	
	
	
	if (!isCompteARebourds_ && !isAdhoc_) {
		for (int i = 0; i < (int) zombies_.size(); i++) {

			if (!zombies_[i]->isDead())
				zombies_[i]->update();
			
			zombies_[i]->setTarget(hero_->getPos());
			zombies_[i]->move();

			if (zombies_[i]->isFinish()) {
				// on a tué un zombie
				kills_ ++;
				increaseScore(10);
				removeZombie(i);
				i--;
			}
		}
	}
	
	
	
		
	if (!isCompteARebourds_)
		hero_->move();
	hero_->updateHuman();
	if (tirTrue_) hero_->updateHuman(); // tir true x2
	
	hero_->update();
	
	if (isAdhoc_) {

		if (hero_->getLife() <= 0) {
			adhoc_->sendEndPacket();
			adhoc_->waitForReplay();
			finish_ = true;
			endGame();
			return;
		}

		// traitement des données adhoc
		while (adhocMsg.size() > 0) {
			if (adhocMsg.at(0).type == PERSO_POS_PACKET) {
				int life, x,y,arme;
				float angle, walkframe;
				sscanf(adhocMsg.at(0).data, "%i,%i,%i,%i,%f,%f", &life, &x,&y,&arme ,&angle, &walkframe );
				setAdhocPlayerValues(life, x,y, arme, angle, walkframe);
			} else if (adhocMsg.at(0).type == PERSO_SHOOT_PACKET) {
				int x,y,arme;
				float angle;
				sscanf(adhocMsg.at(0).data, "%i,%i,%i,%f", &x,&y,&arme ,&angle);
				adhocPlayerShoot(x, y, arme, angle);
			} else if (adhocMsg.at(0).type == ADD_BONUS_PACKET) {
				int x,y,type;
				sscanf(adhocMsg.at(0).data, "%i,%i,%i", &x,&y,&type);
				addBonus(x,y,type);
			} else if (adhocMsg.at(0).type == END_PACKET) {
				adhoc_->waitForReplay();
				endGame();
				finish_ = true;
				return;
			}
			adhocMsg.erase(adhocMsg.begin());
		}
		
		adhocPlayer_->updateHuman();
		adhocPlayer_->update();
	}
	
	if (!isAdhoc_ ) {
		for (int j = 0; j < (int)zombies_.size(); j++) {
				
			if ( pgeMathSqrt(pow(hero_->getPos().x - zombies_[j]->getPos().x,2) + pow( (hero_->getPos().y -50) - ( zombies_[j]->getPos().y - 50),2) ) < 20) {

				if (hero_->getLife() < 2) {
					finish_ = true;
					endGame();
					return;
				}
				hero_->inflictDamage(2, hero_->getPos().x, hero_->getPos().y, 0);
			}
		}
	}
	
	
	pSystem_->update();
	
	if (!isAdhoc_) {
		pSystem_->collide( zombies_);
	} else {
		pSystem_->collide( hero_ );
		pSystem_->collideAdhoc( adhocPlayer_ );
	}
	
	updateCamera();

	if (getHeroLife() < 200 && !pgeWavIsPlaying(heartBreath_)) {
		pgeWavPlay(heartBreath_);
	}
}


void Game::vibrate(int time,float force) {
	
	vibrate_ = true;
	vibrationTime_ += time;
	vibrationForce_ = force;
}

void Game::updateCamera() {
		
	// look at the weapon of the main character
	cameraDest_.x = hero_->getPos().x + pgeMathCos(hero_->getAngleViseur())*150 - 240;
	cameraDest_.y = hero_->getPos().y + pgeMathSin(hero_->getAngleViseur())*50 - 200;

	//collision
	if (cameraDest_.x > 1500 - 480) cameraDest_.x = 1500 - 480;
	if (cameraDest_.x < 0) cameraDest_.x = 0;
	if (cameraDest_.y < 0) cameraDest_.y = 0;
	if (cameraDest_.y > 800 - 272) cameraDest_.y = 900 - 272;
	
	// move camera
	camera_.x += (cameraDest_.x - camera_.x)/10;
	camera_.y += (cameraDest_.y - camera_.y)/10;
	
	ScePspFVector2 camera = camera_;
	
	//vibration
	if (vibrate_) {
		camera.x += pgeMathRandFloat(-vibrationForce_,vibrationForce_);
		camera.y += pgeMathRandFloat(-vibrationForce_,vibrationForce_);
		vibrationTime_--;
		if (vibrationTime_ <= 0)
			vibrate_ = false;
	}
	
	Sprite::setCamera(camera);
	pSystem_->setCamera(camera);
	map_->setCamera(camera);
 }



void Game::draw() {

	map_->draw();
	
	for (int i = 0; i < (int) zombies_.size(); i++) {
		zombies_[i]->draw();
	}
	
	hero_->draw();
	
	if (isAdhoc_) {
		adhocPlayer_->draw();
		intraFontSetStyle(font_, 0.7f, 0xffffffff, 0x00ffffff, 0);
		intraFontPrintf(font_, (int)-camera_.x + adhocPlayer_->getPos().x - intraFontMeasureText(font_,adhocPlayer_->getName())/2, (int)-camera_.y + adhocPlayer_->getPos().y - 140, "%s", adhocPlayer_->getName());
	}

	/// Bonus
	for (int i = 0; i < bonus_.size(); i++) {
		float scale = 0.8 + pgeMathAbs(pgeMathCos((float)bonus_[i].frame/30))*0.4f;
		Sprite::setScale(scale,scale);
		bonusSprite_[bonus_[i].type]->draw2D(bonus_[i].x, bonus_[i].y, 0, 255, false,false);
	}
		
	pSystem_->draw();

	/// Barre de vie
	icons_[1]->draw2DNoCamera(8,8,0,255,false, false);
	pgeRect rect = {17,1,hero_->getLife()*0.15f,10};
	pgeRect rect2 = {17,1,150,10};
	pgeGfxDrawRect(&rect, pgeGfxCreateColor(255-hero_->getLife()*0.255f, hero_->getLife()*0.255f, 0,160), 0);
	pgeGfxDrawRectOutline(&rect2, 0xff000000, 0);
	
	/// Weapon info
	icons_[0]->draw2DNoCamera(8,25,0,255,false, false);
	intraFontSetStyle(font_, 0.8f, 0xffffffff, 0x00ffffff , 0);
	if (hero_->getArme()->getChargeur() > 0) {
		int bullets = pgeMathFmod(hero_->getArme()->getMunitions() , hero_->getArme()->getChargeur() );
		intraFontPrintf(font_, 18, 30, "%i/%i", bullets, hero_->getArme()->getMunitions() - bullets);
	} else {
		intraFontPrint(font_, 18, 30, "-");
	}
	
	/// Time
	icons_[2]->draw2DNoCamera(472,8,0,255,false, false);
	intraFontSetStyle(font_, 0.8f, 0xffffffff, 0x00ffffff, 0);
	char strTime[15];
	sprintf(strTime, Utils::strTime((int)time_) );
	intraFontPrint(font_, 480-intraFontMeasureText(font_,strTime)-16, 13, strTime);

	/// Score
	icons_[3]->draw2DNoCamera(472,25,0,255,false, false);
	intraFontSetStyle(font_, 0.8f, 0xffffffff, 0x00ffffff, 0);
	char strScore[15];
	sprintf(strScore, Utils::virgulesMilliers(scoreDisplay_) );
	intraFontPrintf(font_, 480-intraFontMeasureText(font_,strScore)-16, 30, "%s", strScore);
	
	/// Multiplicateur
	intraFontSetStyle(font_, 0.6f + multiplicateur_*0.15f, pgeGfxCreateColor((float)multiplicateur_*(255/5), 255-(float)multiplicateur_*(255/5), 0,255), 0x00ffffff, 0);
	intraFontPrintf(font_, 240, 15 + multiplicateur_, "x%i", multiplicateur_);

	/// Compte a rebourds
	if (isCompteARebourds_) {
		char text[10];
		if( timeCompteARebourds_ > 50) {
			sprintf(text, "%i", (int)(timeCompteARebourds_/50));
		} else {
			sprintf(text, "Go !");
		}
		intraFontSetStyle(font_, pgeMathFmod(timeCompteARebourds_,50)/20, 0xffffffff, 0x00ffffff, 0);
		intraFontPrint(font_, 240-intraFontMeasureText(font_,text)/2, 136, text);
	}

	/// Tir true
	if (tirTrue_) {
		Sprite::setScale(0.5f,0.5f);
		bonusSprite_[2]->draw2DNoCamera(200,262,0,255,false,false);
		intraFontSetStyle(font_, 0.8f, 0xffffffff, 0x00ffffff, 0);
		intraFontPrintf(font_, 215, 269, "%s",Utils::strTime(tirTrueTime_));
	}
	
}


