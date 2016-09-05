#include <pge/pge.h>
#include <pge/pgeGfx.h>
#include <pge/pgeWav.h>
#include <pge/pgeControls.h>
#include <pge/pgeTimer.h>
#include <pge/pgeUtils.h>
#include <pge/pgeSystem.h>
#include <pge/pgeDir.h>
#include "exceptionHandler/exception.c"
#include "intraFont/intraFont.h"
#include <pspkernel.h> 
#include <pspdebug.h> 
#include <psppower.h> 
#include <vector>

#include "sprite.hpp"
#include "perso.hpp"
#include "human.hpp"
#include "zombie.hpp"
#include "arme.hpp"
#include "particles.hpp"
#include "game.hpp"
#include "map.hpp"
#include "splash.hpp"
#include "menu.hpp"
#include "network.hpp"
#include "stats.hpp"
#include "utils.hpp"
#include "adhoc.hpp"
#include "options.h"

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

/// Ressources
//textures
#include "R/sprites.c"
#include "R/dh1.c"
#include "R/dh2.c"
#include "R/dh3.c"
#include "R/logo.c"
#include "R/logoGorglucks.c"
#include "R/logoScenery.c"
//sons
#include "R/doubleGun.c"
#include "R/douille.c"
#include "R/explosion.c"
#include "R/lanceGrenades.c"
#include "R/laser.c"
#include "R/mitrailleuse.c"
#include "R/shotgun.c"
#include "R/zombie1.c"
#include "R/zombie2.c"
#include "R/zombie3.c"
#include "R/zombie4.c"
#include "R/heartBreath.c"
#include "R/flamme.c"
#include "R/gaz.c"

#include "mp3.h"

enum State {SPLASH,LOAD,MENU,GAME};

State state;
Game* game;
Menu* menu;
Adhoc* adHoc;
Network* network;
Stats* stats;

/// functions
void goToGame() {
	
	
	state = GAME;
	game->initNewGame(false, false);

	menu->setDialog(-1);

	// à cause de l'intraFont
	sceGuDisable(GU_DEPTH_TEST);
}

void getScores() {
		
	network->getScores();
	menu->updateScoreMenu();
}

void goToMenu() {

	state = MENU;
	menu->setDialog(0);
	menu->updateWeaponsMenu();
	adHoc->quit();
}

void goToWeaponsMenu() {
		
	menu->setDialog(4);
}

void goToScoresMenu() {
	
	if (!network->isConnected()) {
		network->internetConnection();
		network->setNextAction(&getScores);
	} else {
		getScores();
	}
	menu->setDialog(5);
	
}

void goToAboutMenu() {
	
	menu->setDialog(9);
}

void goToConfirmation() {

	menu->setDialog(10);
}

void goToAdhocConfirmation() {

	menu->setDialog(18);
}


void goToPause() {

	menu->setDialog(1);
}

void onResume() {
	
	game->setPause(false);
	menu->setDialog(-1);
}

void submitScore() {

	if (!network->isConnected()) {
		network->internetConnection();
		network->setNextAction(&submitScore);
	} else {
		char pseudo[128];
		stats->getPseudo(pseudo);
		network->submitScore(game->getScore(), pseudo );
		network->internetDeconnection();
		goToMenu();
	}
}

void newGame() {
	if( !stats->newSave()) {
		menu->setDialog(3);
	} else {
		goToMenu();
	}
}

void loadGame() {
	if( !stats->load()) {
		menu->setDialog(3);
	} else {
		goToMenu();
		menu->updateWeaponsMenu();
	}
}

void deleteGame() {
	stats->deleteSave();
	menu->setDialog(3);
}

void goToAdhoc() {
	menu->setDialog(6);
}
void adHocCreate() {
	//~ sleepMp3();
	adHoc->create();
	menu->setDialog(7);
}
void adHocJoin() {
	//~ sleepMp3();
	adHoc->join();
	menu->setDialog(8);
}
void adhocServerGoToGame() {
	
	state = GAME;
	game->initNewGame(true, true);

	menu->setDialog(-1);
	sceGuDisable(GU_DEPTH_TEST);
}
void adhocClientGoToGame() {
	
	state = GAME;
	game->initNewGame(true, false);

	menu->setDialog(-1);
	sceGuDisable(GU_DEPTH_TEST);
}

void goToLoadGameMenu() {

	state = LOAD;
	menu->setDialog(3);
}

void goToSettingsMenu() {

	menu->setDialog(14);
}

void adhocReplay() {

	menu->setDialog(15);
	adHoc->replay();
}

void adhocQuit() {

	game->setPause(true);
	if (menu->getDialog() != 0)
		menu->setDialog(16);
}


void goToAdhocPause() {

	menu->setDialog(17);
}

int sleep = 0;

// Power callback
int power_callback(int unknown, int pwrflags, void *common) {

	if (pwrflags & PSP_POWER_CB_SUSPENDING || pwrflags & PSP_POWER_CB_STANDBY) {
		if( !sleep) {
			
			// pause mp3
			sleepMp3();
			sleep = 1;
			
			// quit adhoc
			//~ adHoc->quit();
			
			// shut down net
			pgeNetShutdown();
		}
	}
	
	if (pwrflags & PSP_POWER_CB_RESUME_COMPLETE) {
		sleep = 0;
		resumeMp3();
	}

	return 0;
}

int CallbackThread(SceSize args, void *argp) {
	int cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
	scePowerRegisterCallback(0, cbid);
	sceKernelSleepThreadCB();
	return 0;
}




int main() {

	
	
	// Setup callbacks
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}

	char fileName[100];
	pgeDirGetCwd(fileName, 100);
	sprintf(fileName, "%s/12-2r-scared-zardonic.mp3", fileName);
	
	initMP3(fileName);
	
	pgeWavInit();
	
	state = SPLASH;

	/// Set CPU frequency to 333
	scePowerSetClockFrequency(333, 333, 166);
	
	/// Exception handler
	initExceptionHandler();

	/// Initialisation de PGE
	pgeGfxInit(PGE_PIXEL_FORMAT_8888);
	
	pgeControlsInit();

	/// Network
	network = new Network();
	
	/// Adhoc
	adHoc = new Adhoc(&adhocClientGoToGame, &adhocServerGoToGame, &adhocQuit);

	/// Police
    intraFontInit();
    intraFont* font = intraFontLoad("flash0:/font/ltn0.pgf",0);
    intraFontSetStyle(font, 1.0f, 0xffffffff, 0xff00ff00, 0);
    if (!font) pgeExit();
	
	/// Chargement du son
	std::vector<pgeWav*> crisZombies;
	crisZombies.push_back(pgeWavLoadMemory(R_zombie1,size_R_zombie1));
	crisZombies.push_back(pgeWavLoadMemory(R_zombie2,size_R_zombie2));
	crisZombies.push_back(pgeWavLoadMemory(R_zombie3,size_R_zombie3));
	crisZombies.push_back(pgeWavLoadMemory(R_zombie4,size_R_zombie4));

	pgeWav* wavHeartBreath = pgeWavLoadMemory(R_heartBreath, size_R_heartBreath);
	pgeWav* wavDoubleGun = pgeWavLoadMemory(R_doubleGun,size_R_doubleGun);
	pgeWav* wavDouille = pgeWavLoadMemory(R_douille, size_R_douille);
	pgeWav* wavExplosion = pgeWavLoadMemory(R_explosion, size_R_explosion);
	pgeWav* wavLaser = pgeWavLoadMemory(R_laser, size_R_laser);
	pgeWav* wavLanceGrenades = pgeWavLoadMemory(R_lanceGrenades, size_R_lanceGrenades);
	pgeWav* wavShotgun = pgeWavLoadMemory(R_shotgun, size_R_shotgun);
	pgeWav* wavMitrailleuse = pgeWavLoadMemory(R_mitrailleuse, size_R_mitrailleuse);
	pgeWav* wavFlamme = pgeWavLoadMemory(R_flamme, size_R_flamme);
	pgeWav* wavGaz = pgeWavLoadMemory(R_gaz, size_R_gaz);

	
	// réglage du volume
	pgeWavVolume(wavDoubleGun, 10, 10);
	pgeWavVolume(crisZombies[0], 30, 30);
	pgeWavVolume(crisZombies[1], 30, 30);
	pgeWavVolume(crisZombies[2], 60, 60);
	pgeWavVolume(crisZombies[3], 60, 60);
	pgeWavVolume(wavLaser, 50, 50);
	pgeWavVolume(wavHeartBreath, 100, 100);
	pgeWavVolume(wavDouille, 50, 50);
	
	
	/// Chargement des textures
	pgeTexture* sprites = pgeTextureLoadMemory(R_sprites, size_R_sprites, PGE_RAM, 1);
	if (!sprites) pgeExit();
	pgeTexture* logoDH1 = pgeTextureLoadMemory(R_dh1, size_R_dh1, PGE_RAM, 1);
	if (!logoDH1) pgeExit();
	pgeTexture* logoDH2 = pgeTextureLoadMemory(R_dh2, size_R_dh2, PGE_RAM, 1);
	if (!logoDH2) pgeExit();
	pgeTexture* logoDH3 = pgeTextureLoadMemory(R_dh3, size_R_dh3, PGE_RAM, 1);
	if (!logoDH3) pgeExit();
	pgeTexture* logo = pgeTextureLoadMemory(R_logo, size_R_logo, PGE_RAM, 1);
	if (!logo) pgeExit();
	pgeTexture* logoGorglucks = pgeTextureLoadMemory(R_logoGorglucks, size_R_logoGorglucks, PGE_RAM, 1);
	if (!logoGorglucks) pgeExit();
	pgeTexture* logoScenery = pgeTextureLoadMemory(R_logoScenery, size_R_logoScenery, PGE_RAM, 1);
	if (!logoScenery) pgeExit();
	
	/// Création des sprites
	// armes
	Sprite* spritePacificateur = new Sprite(sprites,240,219,150,50);
	Sprite* spriteMitrailleuse = new Sprite(sprites,240,63,140,51);
	Sprite* spriteDoubleGun = new Sprite(sprites,240,114,140,52);
	Sprite* spriteLanceFlammes = new Sprite(sprites,0,195,174,71);
	Sprite* spriteLaser = new Sprite(sprites,240,166,160,53);
	Sprite* spritePistolet = new Sprite(sprites,80,114,70,42);
	Sprite* spriteMagnum = new Sprite(sprites,159,79,80,38);
	Sprite* spriteLanceGrenades = new Sprite(sprites,240,18,130,45);
	Sprite* spriteShotgun = new Sprite(sprites,240,269,160,53);
	Sprite* spriteGazeur = new Sprite(sprites,240,322,160,77);
	
	// cartouches
	Sprite* cartoucheDoubleGun = new Sprite(sprites, 20,0, 15,5);
	Sprite* cartoucheLaser = new Sprite(sprites, 0,0, 20,7);
	Sprite* cartouchePacificateur = new Sprite(sprites, 35,0, 20,7);
	Sprite* cartoucheMitrailleuse = new Sprite(sprites, 114,102, 20,10);
	Sprite* cartoucheMagnum = new Sprite(sprites, 175,117, 13,7);
	Sprite* cartouchePistolet = new Sprite(sprites, 175,124, 13,7);
	Sprite* cartoucheShotgun = new Sprite(sprites, 188,117, 23,10);
	// bouts de zombies
	std::vector<Sprite*> boutsDeZombies;
	boutsDeZombies.push_back(new Sprite(sprites, 80,156, 30,26));
	boutsDeZombies.push_back(new Sprite(sprites, 110,156,32,22));
	boutsDeZombies.push_back(new Sprite(sprites, 142,156,22,36));
	boutsDeZombies.push_back(new Sprite(sprites, 164,142,32,33));
	boutsDeZombies.push_back(new Sprite(sprites, 189,175,22,28));
	boutsDeZombies.push_back(new Sprite(sprites, 196,141,18,34));
	boutsDeZombies.push_back(new Sprite(sprites, 164,175,25,21));
	// autre
	Sprite* bullet = new Sprite(sprites, 0,7, 70,11);
	Sprite* fire = new Sprite(sprites, 70,0, 18,18);
	Sprite* laserVert = new Sprite(sprites, 88,0,100,18);
	Sprite* laserOrange = new Sprite(sprites, 188,0,100,18);
	Sprite* blood1 = new Sprite(sprites,188,0,20,9);
	Sprite* blood2 = new Sprite(sprites,188,20,20,9);
	Sprite* grenade = new Sprite(sprites,150,117,25,25);
	Sprite* explosion = new Sprite(sprites,0,267,180,134);
	explosion->setScale(1.5f);
	Utils::boxCorner = new Sprite(sprites,216,117,19,19);
	Utils::boxEdge1 = new Sprite(sprites,216,136,19,1);
	Utils::boxEdge2 = new Sprite(sprites,235,117,1,19);
	// icons
	std::vector<Sprite*> icons;
	icons.push_back(new Sprite(sprites,288,0,14,18)); // munitions
	icons.push_back(new Sprite(sprites,302,0,14,18)); // life
	icons.push_back(new Sprite(sprites,316,0,14,18)); // time
	icons.push_back(new Sprite(sprites,330,0,14,18)); // score
	// bonus
	std::vector<Sprite*> bonus;
	bonus.push_back(new Sprite(sprites,195,204,40,40));
	bonus.push_back(new Sprite(sprites,195,284,40,40));
	bonus.push_back(new Sprite(sprites,195,324,40,40));
	bonus.push_back(new Sprite(sprites,195,244,40,40)); //bombe
	
	
	std::vector<Sprite*> explosions;
	for (int i = 0; i < 4; i++) {
		Sprite* explosion = new Sprite(sprites, 0+i*60,18, 60,60);
		explosions.push_back(explosion);
	}
	
	std::vector<Sprite*> spritesHuman;
	spritesHuman.push_back(new Sprite(sprites, 30,95, 39, 33)); // head
	spritesHuman.push_back(new Sprite(sprites, 0, 95, 30, 50)); // body
	spritesHuman.push_back(new Sprite(sprites, 0, 80, 30, 15)); // foot
	spritesHuman.push_back(new Sprite(sprites, 114, 79, 21, 23)); // main 1
	spritesHuman.push_back(new Sprite(sprites, 135, 79, 21, 23)); // main 2
	spritesHuman.push_back(new Sprite(sprites, 69, 79, 35, 35)); // sang
	spritesHuman.push_back(new Sprite(sprites, 412, 0, 33, 25)); // sang 2
	
	std::vector<Sprite*> spritesZombie;
	spritesZombie.push_back(new Sprite(sprites, 0, 145, 50,50));// head
	spritesZombie.push_back(new Sprite(sprites, 50, 145, 30, 50)); //body
	spritesZombie.push_back(new Sprite(sprites, 30, 80, 30, 15)); //foot	
	spritesZombie.push_back(new Sprite(sprites, 370, 0, 42, 22)); //main1
	spritesZombie.push_back(new Sprite(sprites, 370, 22, 42, 18));  //main2
	spritesZombie.push_back(new Sprite(sprites, 69, 79, 35, 35)); // sang
	spritesZombie.push_back(new Sprite(sprites, 412, 0, 33, 25)); // sang 2
 
	/// Map
	Map* map = new Map();
	
	/// Création des particules
	Particles* pSystem = new Particles(map, explosion);
	pSystem->addSprite(fire);
	pSystem->addSprite(blood1);
	pSystem->addSprite(blood2);
	pSystem->addSound(wavDouille);
	pSystem->addSound(wavExplosion);
	
	/// Création des armes
	Mitrailleuse* mitrailleuse  = new Mitrailleuse(pSystem, spriteMitrailleuse, bullet, explosions, cartoucheMitrailleuse, wavMitrailleuse);
	Laser* laser  = new Laser(pSystem, spriteLaser, laserVert, explosions, cartoucheLaser, wavLaser);
	DoubleGun* doubleGun = new DoubleGun(pSystem, spriteDoubleGun, bullet, explosions, cartoucheDoubleGun, wavDoubleGun);
	Pistolet* pistolet = new Pistolet(pSystem, spritePistolet, bullet, explosions, cartouchePistolet, wavDoubleGun);
	LanceFlammes* lanceFlammes = new LanceFlammes(pSystem, spriteLanceFlammes, fire, wavFlamme);
	Pacificateur* pacificateur  = new Pacificateur(pSystem, spritePacificateur, laserOrange, explosions, cartouchePacificateur, wavDoubleGun);
	Magnum* magnum  = new Magnum(pSystem, spriteMagnum, bullet, explosions, cartoucheMagnum, wavDoubleGun);
	LanceGrenades* lanceGrenades = new LanceGrenades(pSystem, spriteLanceGrenades, grenade, explosions, wavLanceGrenades);
	Shotgun* shotgun = new Shotgun(pSystem, spriteShotgun, bullet, explosions, cartoucheShotgun, wavShotgun);
	Gazeur* gazeur = new Gazeur(pSystem, spriteGazeur, fire, wavGaz);
	
	std::vector<Arme*> armes;
	armes.push_back(pistolet);
	armes.push_back(mitrailleuse);
	armes.push_back(doubleGun);	
	armes.push_back(magnum);
	armes.push_back(shotgun);
	armes.push_back(lanceGrenades);
	armes.push_back(laser);
	armes.push_back(lanceFlammes);
	armes.push_back(pacificateur);
	armes.push_back(gazeur);
	
	/// Stats
	stats = new Stats(armes);
	
	/// Création du personnage (Humain)
	Human* human = new Human(240, 50, spritesHuman, pSystem, map, stats, adHoc, false, "");

	/// Création d'un zombie
	Zombie* zombie = new Zombie(0,0, spritesZombie, boutsDeZombies, pSystem, map, crisZombies);
	
	/// Creation du jeu
	game = new Game(map, font, stats, icons, bonus, adHoc, wavHeartBreath);
	
	// Ajout des personnages
	game->setHuman(human);
	game->setZombie(zombie);
	
	// Ajout du particleSystem
	game->setParticleSystem(pSystem);
	
	for (int i = 0; i < 10; i++)
		armes[i]->setGame(game);

	adHoc->setGame(game);

	/// Menu
	menu = new Menu(font, font, logo, stats, network, adHoc);

	// menu principal
	menu->addDialog("Main Menu", MAIN_MENU);
	menu->addItem(0, "Solo", &goToGame);
	menu->addItem(0, "AdHoc", &goToAdhoc);
	//~ menu->addItem(0, "AdHoc", NULL);
	menu->addItem(0, "Ranking", &goToScoresMenu);
	//~ menu->addItem(0, "Ranking", NULL);
	menu->addItem(0, "Weapons", &goToWeaponsMenu);
	menu->addItem(0, "Settings", &goToSettingsMenu);
	menu->addItem(0, "About", &goToAboutMenu);

	// menu pause
	menu->addDialog("Paused", NORMAL);
	menu->addItem(1, "Resume", &onResume);
	menu->addItem(1, "Back to menu", &goToConfirmation);

	// menu game over
	menu->addDialog("Game Over !",NORMAL);
	menu->addItem(2, "Submit score", &submitScore);
	menu->addItem(2, "Back to menu", &goToConfirmation);

	// menu load
	menu->addDialog("Load Game",NORMAL);
	menu->addItem(3, "New game", &newGame);
	menu->addItem(3, "Load game", &loadGame);
	menu->addItem(3, "Delete game", &deleteGame);
	
	//weapons menu
	menu->addDialog("Weapons",WEAPONS);
	menu->updateWeaponsMenu();
	
	//scores menu
	menu->addDialog("Scores",SCORES);
	menu->updateScoreMenu();

	//adHoc menu (create or join)
	menu->addDialog("Adhoc",NORMAL);
	menu->addItem(6, "Create game", &adHocCreate);
	menu->addItem(6, "Join game", &adHocJoin);

	//adHoc menu server
	menu->addDialog("Waiting for player...",ADHOC_SERVER);

	//adHoc menu client
	menu->addDialog("Choose game",ADHOC_CLIENT);

	//About menu
	menu->addDialog("About",ABOUT);
	menu->addItem(9,"",NULL);

	// Confirmation de retour
	menu->addDialog("Are you sure ?",NORMAL);
	menu->addItem(10, "Yes", &goToMenu);
	menu->addItem(10, "No", &goToPause);

	// Confirmation de retour au menu "load game"
	menu->addDialog("Return to Load game ?",NORMAL);
	menu->addItem(11, "Yes", &goToLoadGameMenu);
	menu->addItem(11, "No", &goToMenu);

	// menu adhoc win
	menu->addDialog("You win !",NORMAL);
	menu->addItem(12, "Replay", &adhocReplay);
	menu->addItem(12, "Return to menu", &goToMenu);

	// menu adhoc loose
	menu->addDialog("You lose !",NORMAL);
	menu->addItem(13, "Replay", &adhocReplay);
	menu->addItem(13, "Return to menu", &goToMenu);

	//menu options
	menu->addDialog("Settings",NORMAL);
	menu->addItem(14, "< Music volume : 50 >", NULL);
	menu->addItem(14, "< Controls : Mode 1 >", NULL);
	menu->addItem(14, "Edit Pseudo", NULL);

	// wait for replay menu
	menu->addDialog("Waiting for opponent...",NORMAL);
	menu->addItem(15, "", NULL);

	// wait for replay menu
	menu->addDialog("Your opponent has left",NORMAL);
	menu->addItem(16, "Return to menu", &goToMenu);

	// menu pause adhoc
	menu->addDialog("Adhoc", NORMAL);
	menu->addItem(17, "Resume", &onResume);
	menu->addItem(17, "Back to menu", &goToAdhocConfirmation);

	// menu confirmation adhoc
	menu->addDialog("Are you sure", NORMAL);
	menu->addItem(18, "Yes", &goToMenu);
	menu->addItem(18, "No", &goToAdhocPause);
	
	
	/// variables pour le calcul des fps
	float dt = 1;
	pgeTimer* timer = pgeTimerCreate();
	
	pspDebugScreenInit();
		
	
	/// //////////////////////////////////////////////////////////// ///
	/// Splash Screen ///
	state = SPLASH;
	Splash* splash = new Splash(logoDH1,logoDH2,logoDH3,logoGorglucks,logoScenery, font);
	while (pgeRunning() && !splash->isFinish()) {
		splash->update();
		splash->draw();
	}
	delete splash;
	pgeTextureDestroy(logoDH1);
	pgeTextureDestroy(logoDH2);
	pgeTextureDestroy(logoDH3);
	pgeTextureDestroy(logoGorglucks);
	pgeTextureDestroy(logoScenery);
	/// //////////////////////////////////////////////////////////// ///
	
	
	state = LOAD;
	
	menu->setDialog(3);

	int addZombie = 0;

	
	while (pgeRunning()) {
		
		pgeControlsUpdate();
		
		// calcul fps
		dt = pgeTimerGetDeltaTime(timer);
		pgeTimerUpdate(timer);

		if (state == GAME) {

			game->update();

			if (game->addZombie() && game->getNumZombies() < 20 && !game->isAdhoc()) {

				float x,y;
				do {
					x = pgeMathRandFloat(100,1400);
					y = pgeMathRandFloat(100,800);
				} while( !map->collideX(x,y) || pgeMathSqrt(pow(game->getHeroPos().x - x, 2) + pow( game->getHeroPos().y - y, 2) ) < 600 );
				
				Zombie* z = new Zombie(x,y, spritesZombie, boutsDeZombies, pSystem, map, crisZombies);
				z->setPos(x,y);
				float speed = 0.5f + (float)game->getNumZombiesKilled() / 300;
				if (speed > 5) speed = 5;
				speed += pgeMathRandFloat(-1,1);
				if (speed < 1) speed = 1;
				
				z->setSpeed(speed);
				game->addZombie(z);
			}


			if (game->getFinish() && menu->getDialog() != 10 && menu->getDialog() != 15) {
				// on s'est fait mangé...
				game->setPause(true);
				if (game->isAdhoc()) {

					if (game->getHeroLife() <= 0) {
						menu->setDialog(13); // perdu
					} else {
						menu->setDialog(12); // gagne
					}
					
				} else {
					menu->setDialog(2);
				}
			}

			/// La pause
			if (pgeControlsPressed(PGE_CTRL_START)) {
				
				if (!game->isAdhoc()) {
					if (!game->getPause()) {
						game->setPause(true);
						menu->setDialog(1);
					} else {
						game->setPause(false);
						menu->setDialog(-1);
					}
				} else {
					if (menu->getDialog() == -1) {
						menu->setDialog(17);
					} else {
						menu->setDialog(-1);
					}
				}
			}
		}

		if (!network->dialog())
			menu->update();
		
		
		adHoc->update();
		
		//-------------------- Draw ----------------------------------//
		
		pgeGfxStartDrawing();
		pgeGfxClearScreen(0);
		
		if (state == GAME) {
		
			game->draw();
		}

		menu->draw();

		/// debugging
		//~ pspDebugScreenSetXY(0,0);
		//~ if (dt != 0) pspDebugScreenPrintf("%f\n", 1/dt );
		
		
		adHoc->draw();

		if (network->dialog()) {
			pgeRect screen = {0,0,480,272};
			pgeGfxDrawRect(&screen,0xaa000000,0);
		}
		pgeGfxEndDrawing();
		network->draw();
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);

	}

	// cleaning time
	pgeWavShutdown();
	pgeNetShutdown();
	pgeGfxShutdown();
	pgeExit();
	
    return 0;
}
