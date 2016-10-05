#ifndef SKULLGROG_ADHOC_H
#define SKULLGROG_ADHOC_H

#include <pge/pgeControls.h>
#include <pge/pgeMath.h>
#include <pge/pgeNet.h>
#include <pge/pgeSystem.h>
#include <pspkernel.h>
#include "system/adhoc.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "game.hpp"
#include "utils.hpp"


enum AdhocState {
	NOTHING,
	SERVER_WAIT_FOR_PLAYER,
	CLIENT_WAIT_FOR_GAME,
	SERVER_IN_GAME,
	CLIENT_IN_GAME,
	SERVER_WAIT_FOR_REPLAY,
	CLIENT_WAIT_FOR_REPLAY
};

#define DATA_SIZE 30
#define PERSO_POS_PACKET 1
#define PERSO_SHOOT_PACKET 2
#define ADD_BONUS_PACKET 3
#define END_PACKET 4
#define REPLAY_PACKET 5

typedef struct AdhocDataPacket {
	char type;
	char data[DATA_SIZE];
} AdhocDataPacket;


class Adhoc {
	
	private :

		Game* game_;

		char hello_[128];
		char* product_;
		int rand_val_;
		
		bool server_;
		bool serverstarted_;

		AdhocState state_;

		void (*clientStartGame_)();
		void (*serverStartGame_)();
		void (*quit_)();

		int sendDelay_;

		int test_;
		int n_;

		bool replay_;
		bool sendReplay_;

		bool init_;
	
	
	public :
	
		Adhoc(void clientStartGame(), void serverStartGame(), void quit());

		void setGame(Game* game);
		bool getServer();

		
		// Callbacks
		static void clientCallback(void* pointerData, adhoc::PSPelement* psp, int datalen, void *data);
		static void serverCallback(void* pointerData, adhoc::PSPelement* psp, int datalen, void *data);
		static void testCallback(void* pointerData, adhoc::PSPelement* psp, int datalen, void *data);

		// Serveur
		void create();
		void serverDisconnect();
		void serverSend(char* data);
		void addBonus(int x, int y, int type);

		// Client
		void join();
		void clientDisconnect();
		void requestConnection(int server);
		void cancelConnection();
		void disconnect();
		void clientSend(char* data);

		void shoot(int x,int y, int arme, float angle);
		void sendEndPacket();
		void waitForReplay();
		void replay();
		void quit();

		void update();
		void draw();

	
};

#else

class Adhoc;

#endif


