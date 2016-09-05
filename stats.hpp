#ifndef STATS_H
#define STATS_H

#include <sstream>
#include <pge/pgeNet.h>
#include <pge/pgeUtils.h>
#include <pge/pgeGfx.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include "utils.hpp"
#include "arme.hpp"
#include <vector>

// Armes
#include "Armes/mitrailleuse.hpp"
#include "Armes/lanceFlammes.hpp"
#include "Armes/doubleGun.hpp"
#include "Armes/laser.hpp"
#include "Armes/pacificateur.hpp"
#include "Armes/pistolet.hpp"
#include "Armes/magnum.hpp"
#include "Armes/lanceGrenades.hpp"
#include "Armes/shotgun.hpp"
#include "Armes/gazeur.hpp"


typedef struct Weapon {
	
	char* name;
	char* unlocking;
	int power;
	int speed;
	int unlocked;
	
} Weapon;

//~ typedef struct StatScore {
	//~ char* name;
	//~ int score;
//~ } StatScore;


class Stats {

	private :

		const char* key_;
		
		std::ostringstream savedata_;
		
		char loaddata_[256];
		
		// Armes
		Pistolet* pistolet_;
		Mitrailleuse* mitrailleuse_;
		DoubleGun* doubleGun_;
		Magnum* magnum_;
		Shotgun* shotgun_;
		Laser* laser_;
		LanceGrenades* lanceGrenades_;
		LanceFlammes* lanceFlammes_;
		Pacificateur* pacificateur_;
		Gazeur* gazeur_;

		std::vector<Arme*> armes_;
		
		//////////////
		int kills_;
		int time_;
		int games_;
		Weapon weapons_[10];
		//~ std::vector<StatScore> scores_;
		char pseudo_[128];
		int controls_;
		//////////////
		
		
		

	public :
	
		Stats(std::vector<Arme*>& armes);
		~Stats();

		bool newSave();
		bool load();
		void save();
		void deleteSave();
		
		Weapon getWeapon(int index);
		Arme* getArme(int index);
		void getPseudo(char* pseudo);
		void setPseudo(char* pseudo);
		void setPersoWeapons(std::vector<Arme*>* weapons, bool adhoc);
		int getControls();
		void setControls(int controls);

		//~ void saveLocalScore(int score);

		bool checkUnlocks(int kills, int score, int time);

		void kills(int kills);
		void time(int time);
		void games(int games);

		void update();

		void draw();

		

};

#else

class Stats;

#endif
