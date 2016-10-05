#ifndef GAME_H
#define GAME_H


#include <pge/pgeTimer.h>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>
#include <pge/pgeControls.h>
#include <pge/pgeMp3.h>
#include <vector>
#include <stdio.h>
#include "intraFont/intraFont.h"

#include "sprite.hpp"
#include "arme.hpp"
#include "human.hpp"
#include "zombie.hpp"
#include "stats.hpp"
#include "adhoc.hpp"

class Perso;
class Particles;
class Map;

#define VIE 0
#define MUNITIONS 1
#define TIR_TRUE 2
#define BOMBE 3

typedef struct GameAdhocData {
	char type;
	char* data;
};	

typedef struct Bonus {
	float x, y;
	int type;
	int frame;
	
} Bonus;

struct AdhocDataPacket;



class Game {
	
	private :

		Zombie* zombie_; // zombie de reference
		/// Les zombies
		std::vector<Zombie*> zombies_;

		Human* human_; // humain de référence
		Human* hero_;
		Human* adhocPlayer_;
		
		/// Les bonus
		std::vector<Bonus> bonus_;
		std::vector<Sprite*> bonusSprite_;
		
		/// Le particle system
		Particles* pSystem_;

		/// Police
		intraFont* font_;
		
		/// La map
		Map* map_;

		pgeWav* heartBreath_;

		Stats* stats_;
		
		ScePspFVector2 camera_;
		ScePspFVector2 cameraDest_;

		int addZombie_;
		int addBonus_;

		int numZombies_;
		
		// vibration
		bool vibrate_;
		int vibrationTime_;
		float vibrationForce_;

		///
		bool pause_;
		bool finish_;
		float time_;
		pgeTimer* timer_;

		// compte a rebourds
		bool isCompteARebourds_;
		int timeCompteARebourds_;

		bool tirTrue_;
		int tirTrueTime_;
		
		int kills_;
		int score_;
		float scoreDisplay_;
		int multiplicateur_;
		int multiplicateurTime_;
		int multiplicateurKills_;
		
		// icones 
		std::vector<Sprite*> icons_;

		/// Adhoc ///
		Adhoc* adhoc_;
		bool isAdhoc_;
		bool server_;

		int debug_;

	public :

		static std::vector<AdhocDataPacket> adhocMsg;

		Game(Map* map, intraFont* font, Stats* stats_, std::vector<Sprite*> icons, std::vector<Sprite*> bonus, Adhoc* adhoc, pgeWav* heartBreath);
		~Game();

		void initNewGame(bool adhoc, bool server);
		void endGame();
		void replay();

		void setZombie(Zombie* zombie);
		bool addZombie();
		void addZombie(Zombie* zombie);
		void addZombie(int x,int y,float speed);
		void setHuman(Human* human);
		void setParticleSystem(Particles* pSystem);
		void setPause(bool state);
		bool isAdhoc();
		
		Zombie* getZombie(int index);
		bool getPause();
		ScePspFVector2 getCamera();
		int getScore();

		Map* getMap();
		
		std::vector<Perso*>* getPersos();

		void removeZombie(int index);
		
		int getNumZombies();
		int getNumZombiesKilled();
		int getHeroLife();
		ScePspFVector2 getHeroPos();
		float getHeroAngle();
		void setAdhocPlayerValues(int life, int x,int y, int arme,float angle, float walkFrame);
		void adhocPlayerShoot(int x, int y, int arme, float angle);
		Human* getHero();
		
		bool getFinish();
		
		void addBonus();
		void addBonus(int x, int y, int type);
		void bonusCollide();
		void removeBonus(int bonus);
		
		void vibrate(int time, float force);

		void increaseScore(int points);

		void updateCamera();
		void update();
		
		void draw();

	
};

#else

class Game;

#endif
