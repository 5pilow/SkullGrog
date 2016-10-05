#include "stats.hpp"


#define NUM_WEAPONS 10

Stats::Stats(std::vector<Arme*>& armes) {

	key_ = "kamouloxkamoulox";

	kills_ = -1;
	time_ = -1;
	games_ = -1;
	
	/// Armes
	//pistolet
	weapons_[0].name = "Gun";
	weapons_[0].unlocking = "---";
	weapons_[0].power = 1;
	weapons_[0].speed = 4;
	//mitrailleuse
	weapons_[1].name = "Machine gun";
	weapons_[1].unlocking = "Kill 10 zombies";
	weapons_[1].power = 4;
	weapons_[1].speed = 2;
	//double gun
	weapons_[2].name = "Double gun";
	weapons_[2].unlocking = "Get a score of 10.000 or more in one game";
	weapons_[2].power = 3;
	weapons_[2].speed = 4;
	//magnum
	weapons_[3].name = "Magnum";
	weapons_[3].unlocking = "Play one hour";
	weapons_[3].power = 3;
	weapons_[3].speed = 3;
	//fusil à pompe
	weapons_[4].name = "Shotgun";
	weapons_[4].unlocking = "Get a score of 100.000 or more in one game";
	weapons_[4].power = 4;
	weapons_[4].speed = 2;
	//lance grenades
	weapons_[5].name = "Nade launcher";
	weapons_[5].unlocking = "Kill 5.000 zombies";
	weapons_[5].power = 4;
	weapons_[5].speed = 4;
	//laser
	weapons_[6].name = "Laser";
	weapons_[6].unlocking = "Kill 10.000 zombies";
	weapons_[6].power = 4;
	weapons_[6].speed = 4;
	
	//lance flammes
	weapons_[7].name = "Flamethrower";
	weapons_[7].unlocking = "Kill 50.000 zombies";
	weapons_[7].power = 4;
	weapons_[7].speed = 5;
	//pacificateur
	weapons_[8].name = "Blaster";
	weapons_[8].unlocking = "Kill 5.000 zombies in one game";
	weapons_[8].power = 5;
	weapons_[8].speed = 4;
	//gazeur
	weapons_[9].name = "Gas thrower";
	weapons_[9].unlocking = "Kill 100.000 zombies";
	weapons_[9].power = 5;
	weapons_[9].speed = 5;
	
	for (int i = 0; i < NUM_WEAPONS; i++) {
		
		weapons_[i].unlocked = (i == 0) ? 1 : 0;
	}
	
	pistolet_ = (Pistolet*) armes[0];
	mitrailleuse_ = (Mitrailleuse*) armes[1];
	doubleGun_ = (DoubleGun*) armes[2];
	magnum_ = (Magnum*) armes[3];
	shotgun_ = (Shotgun*) armes[4];
	lanceGrenades_ = (LanceGrenades*) armes[5];
	laser_ = (Laser*) armes[6];
	lanceFlammes_ = (LanceFlammes*) armes[7];
	pacificateur_ = (Pacificateur*) armes[8];
	gazeur_ = (Gazeur*) armes[9];

	armes_ = armes;

	//~ for (int i = 0; i < 10; i++) {
		//~ StatScore score = {"---", 0};
		//~ scores_.push_back(score);
	//~ }

	controls_ = 0;
}

Stats::~Stats() {}

void Stats::kills(int kills) {

	kills_ += kills;
}

void Stats::time(int time) {

	time_ += time;
}
void Stats::games(int games) {

	games_ += games;
}

Weapon Stats::getWeapon(int index) {
		
	return weapons_[index];
}

void Stats::getPseudo(char* pseudo) {

	sprintf(pseudo, pseudo_);
}

void Stats::setPseudo(char* pseudo) {

	sprintf(pseudo_, pseudo);
}	

Arme* Stats::getArme(int index) {

	return armes_[index];
}

int Stats::getControls() {

	return controls_;
}

void Stats::setControls(int controls) {

	controls_ = controls;
}


void Stats::setPersoWeapons(std::vector<Arme*>* weapons, bool adhoc) {

	if (getWeapon(0).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Pistolet(*pistolet_));
	if (getWeapon(1).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Mitrailleuse(*mitrailleuse_));
	if (getWeapon(2).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new DoubleGun(*doubleGun_));
	if (getWeapon(3).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Magnum(*magnum_));
	if (getWeapon(4).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Shotgun(*shotgun_));
	if (getWeapon(5).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new LanceGrenades(*lanceGrenades_));
	if (getWeapon(6).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Laser(*laser_));
	if (getWeapon(7).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new LanceFlammes(*lanceFlammes_));
	if (getWeapon(8).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Pacificateur(*pacificateur_));
	if (getWeapon(9).unlocked == 1 || adhoc)
		weapons->push_back((Arme*)new Gazeur(*gazeur_));

}

bool Stats::newSave() {

	kills_ = -1;

	sprintf(pseudo_, Utils::osk("Enter you pseudo",Utils::getPSPPseudo()));

	controls_ = 0;

	char savedata[256];
	sprintf(savedata, "0,0,0, 1,0,0,0,0,0,0,0,0,0, %s",pseudo_);

	pgeUtilsSavedataInit(PGE_UTILS_SAVEDATA_TYPE_LISTSAVE, &savedata, 256, "SKULLGROG", key_ , "Skullgrog", pseudo_, "Weapons : 1/10\nZombies killed : 0\nPlay time : 0\nGames played : 0" );

	while (true) {
		pgeGfxStartDrawing();
		pgeGfxClearScreen(0);
		pgeGfxEndDrawing();
		int result = pgeUtilsSavedataUpdate();
		if (result > -1) {
			if (result == 0) {
				kills_ = 0;
				time_ = 0;
				games_ = 0;
				return true;
			} else {
				return false;
			}
		}
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	}
}

void Stats::save() {

	savedata_.str("");
	savedata_ << kills_ << "," << time_ << "," << games_;
	for (int i = 0; i < NUM_WEAPONS; i++)
		savedata_ << "," << weapons_[i].unlocked;
	savedata_ << "," << pseudo_;
	//~ for (int i = 0; i < 10; i++) {
	//~ }
	
	int weapons = 0;
	for (int i = 0; i < 10; i++) {
		if (weapons_[i].unlocked == 1) weapons++ ;
	}
	
	std::ostringstream details;
	details << "Weapons : " << weapons << "/10\nZombies killed : " << kills_ << "\nPlay time : " << Utils::strTime(time_) << "\nGames played : " << games_;
	
	
	char savedata[256];
	sprintf(savedata, savedata_.str().c_str());

	pgeUtilsSavedataInit(PGE_UTILS_SAVEDATA_TYPE_AUTOSAVE, &savedata, 256, "SKULLGROG", key_ , "Skullgrog", pseudo_, details.str().c_str());
	while (true) {
		pgeGfxStartDrawing();
		pgeGfxEndDrawing();
		if(pgeUtilsSavedataUpdate() > -1) return;
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	}
}

bool Stats::load() {

	kills_ = -1;

	pgeUtilsSavedataInit(PGE_UTILS_SAVEDATA_TYPE_LISTLOAD, &loaddata_, 256, "SKULLGROG", key_,"","","");

	while (true) {
		pgeGfxStartDrawing();
		pgeGfxClearScreen(0);
		pgeGfxEndDrawing();
		int result = pgeUtilsSavedataUpdate();
		if (result == 0) {
			// chargement des valeurs
			sscanf(loaddata_, "%i,%i,%i, %i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &kills_,&time_,&games_,&weapons_[0].unlocked,&weapons_[1].unlocked,&weapons_[2].unlocked,&weapons_[3].unlocked,&weapons_[4].unlocked,&weapons_[5].unlocked,&weapons_[6].unlocked,&weapons_[7].unlocked,&weapons_[8].unlocked,&weapons_[9].unlocked);
			// pseudo
			int pos = 0, item = 0;
			while (item < 13) if (loaddata_[pos++] == ',') item++;
			std::string str(loaddata_+pos);
			sprintf(pseudo_, str.c_str());
			
			return true;
		} else if (result == 1) {
			return false; // Fail to load the data.
		}
		
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	}
}

void Stats::deleteSave() {

	pgeUtilsSavedataInit(PGE_UTILS_SAVEDATA_TYPE_LISTDELETE, &loaddata_, 256, "SKULLGROG", key_,"","","");

	while (true) {
		pgeGfxStartDrawing();
		pgeGfxClearScreen(0);
		pgeGfxEndDrawing();
		if (pgeUtilsSavedataUpdate() > -1) {
			return;
		}
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	}
}

//~ void Stats::saveLocalScore(int score) {
//~ 
	//~ StatScore statScore = {pseudo_, score};
//~ 
	//~ for (int i = 0; i < 10; ++i) {
		//~ if (score > scores_[i].score) {
			//~ scores_.insert(scores_.begin() + i, statScore);
			//~ break;
		//~ }
	//~ }
	//~ scores_.pop_back();
//~ }

void Stats::draw() {

}

bool Stats::checkUnlocks(int kills, int score, int time) {
	
	bool unlocked = false;	
	
	// mitrailleuse
	if (1 != weapons_[1].unlocked && kills_ >= 10) {
		weapons_[1].unlocked = 1;
		unlocked = 1;
	}
	//double gun
	if (1 != weapons_[2].unlocked && score >= 10000) {
		weapons_[2].unlocked = 1;
		unlocked = 1;
	}
	//magnum
	if (1 != weapons_[3].unlocked && time_ >= 3600 /* 1h */) {
		weapons_[3].unlocked = 1;
		unlocked = 1;
	}
	//fusil à pompe
	if (1 != weapons_[4].unlocked && score >= 100000) {
		weapons_[4].unlocked = 1;
		unlocked = 1;
	}
	//lance grenades
	if (1 != weapons_[5].unlocked && kills_ >= 5000) {
		weapons_[5].unlocked = 1;
		unlocked = 1;
	}
	//laser
	if (1 != weapons_[6].unlocked && kills_ >= 10000) {
		weapons_[6].unlocked = 1;
		unlocked = 1;
	}
	//lance flammes
	if (1 != weapons_[7].unlocked && kills_ >= 50000) {
		weapons_[7].unlocked = 1;
		unlocked = 1;
	}
	//pacificateur
	if (1 != weapons_[8].unlocked && kills >= 5000) {
		weapons_[8].unlocked = 1;
		unlocked = 1;
	}
	//gazeur
	if (1 != weapons_[9].unlocked && kills_ >= 100000) {
		weapons_[9].unlocked = 1;
		unlocked = 1;
	}
	return unlocked;
}
