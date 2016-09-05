#ifndef HUMAN_H
#define HUMAN_H

#include <pge/pgeControls.h>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>
#include <pge/pgeWav.h>

#include "arme.hpp"
#include "perso.hpp"
class Particles;
class Stats;
//~ class Adhoc;
#include "adhoc.hpp"

// Armes
class Mitrailleuse;
class LanceFlammes;
class DoubleGun;
class Laser;
class Pacificateur;
class Gazeur;
class Shotgun;
class LanceGrenades;
class Pistolet;
class Magnum;
//~ #include "Armes/mitrailleuse.hpp"
//~ #include "Armes/lanceFlammes.hpp"
//~ #include "Armes/doubleGun.hpp"
//~ #include "Armes/laser.hpp"
//~ #include "Armes/pacificateur.hpp"
//~ #include "Armes/pistolet.hpp"
//~ #include "Armes/magnum.hpp"
//~ #include "Armes/lanceGrenades.hpp"
//~ #include "Armes/shotgun.hpp"
//~ #include "Armes/gazeur.hpp"


class Human : public Perso {
	
	private :

		Adhoc* adhoc_;

		//armes du joueur
		std::vector<Arme*> armes_;
		// arme en cours
		Arme* arme_;
		int currentArme_;
		bool isAdhoc_;
		
		// Stats (pour les armes débloquées)
		Stats* stats_;

		char* name_;

		std::vector<Sprite*> sprites_;
		
	public :
	
		Human(float x, float y, std::vector<Sprite*>& sprites, Particles* pSystem, Map* map, Stats* stats, Adhoc* adhoc, bool isAdhoc, char* name);
		~Human();
	
		void addArme(Arme* arme);
		void setArme(Arme* arme);
		void setArmeID(int id);
		Arme* getArme();
		void shoot();

		char* getName();
		std::vector<Sprite*>& getSprites();
		
		void reset(bool adhoc);
		
		void inflictDamage(float damage,float x, float y, float angle);
		void addLife(int life);
		
		void addMunitions();
		
		void move();
		void updateHuman();
		
		void draw();
		void drawArme();
		
		
		
		
		
	
};

#else

class Human;

#endif
