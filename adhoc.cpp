#include "adhoc.hpp"
#include "system/adhoc.h"

int n = 0;
char msg[128];

void Adhoc::clientCallback(void* pointerData, adhoc::PSPelement* psp, int datalen, void *data) {
	AdhocDataPacket* p = (AdhocDataPacket*) data;
	Game::adhocMsg.push_back(*p);
}

void Adhoc::serverCallback(void* pointerData, adhoc::PSPelement* psp, int datalen, void *data) {
	AdhocDataPacket* p = (AdhocDataPacket*) data;
	Game::adhocMsg.push_back(*p);
}

int test = 0;
void Adhoc::testCallback(void* pointerData, adhoc::PSPelement* psp, int datalen, void *data) { test++; }

Adhoc::Adhoc(void clientStartGame(), void serverStartGame(), void quit()) {
	
	char nickname[128];
	pgeSystemGetNickname(nickname);
	sprintf(hello_, "%s", nickname);
	product_ = "SKULLGROG";

	clientStartGame_ = clientStartGame;
	serverStartGame_ = serverStartGame;
	quit_ = quit;

	sendDelay_ = 0;
	state_ = NOTHING;

	test_ = 0;
	n_ = 0;

	replay_ = false;
	sendReplay_ = false;

	init_ = false;

	
}

void Adhoc::quit() {

	if (server_)
		serverDisconnect();
	else
		clientDisconnect();
}


void Adhoc::setGame(Game* game) {

	game_ = game;
} 

void Adhoc::create() {

	/// Init server
	pgeNetShutdown();

	if (!init_) {
		adhoc::init(product_);
		init_ = true;
	}
	
	int error;
	
	if (error >= 0) error = adhoc::server::start(strlen(hello_)+1,(void*)hello_);
	if (error < 0) {}

	adhoc::server::setClientConnectedCallback(testCallback, this);
	adhoc::server::setcallback(serverCallback,this);
	adhoc::server::setAcceptationMode(adhoc::SERVER_MODE_AUTO_ACCEPT);

	server_ = true;
	serverstarted_ = true;
	state_ = SERVER_WAIT_FOR_PLAYER;
}

void Adhoc::serverDisconnect() {

	state_ == NOTHING;
	adhoc::server::stop();
}

void Adhoc::join() {

	/// Init client
	pgeNetShutdown();

	if (!init_) {
		adhoc::init(product_);
		init_ = true;
	}

	int error;
	if (error >= 0) error = adhoc::client::start(strlen(hello_)+1,(void*)hello_);
	if (error < 0) {}
	adhoc::client::setcallback(clientCallback,this);

	server_ = false;
	serverstarted_ = false;
	state_ = CLIENT_WAIT_FOR_GAME;
}

void Adhoc::clientDisconnect() {

	state_ == NOTHING;
	adhoc::client::disconnect();
	adhoc::client::stop();
}

void Adhoc::requestConnection(int server) {

	adhoc::client::requestConnection(adhoc::client::getServerList().at(server));
}

void Adhoc::cancelConnection() {

	adhoc::client::cancelRequestedConnection();
}

void Adhoc::disconnect() {

	adhoc::client::disconnect();
}


void Adhoc::addBonus(int x, int y, int type) {
	AdhocDataPacket packet;
	packet.type = ADD_BONUS_PACKET;
	sprintf(packet.data, "%i,%i,%i", x,y,type);
	adhoc::PSPelement* psp = adhoc::server::getClientList().at( 0 );
	adhoc::server::send(psp, sizeof(AdhocDataPacket), &packet);
}

void Adhoc::shoot(int x,int y, int arme, float angle) {

	AdhocDataPacket packet;
	packet.type = PERSO_SHOOT_PACKET;
	sprintf(packet.data, "%i,%i,%i,%f", x,y,arme,angle);

	if (server_) {
		adhoc::PSPelement* psp = adhoc::server::getClientList().at( 0 );
		adhoc::server::send(psp, sizeof(AdhocDataPacket), &packet);
	} else {
		adhoc::client::send(sizeof(AdhocDataPacket), &packet);
	}
	
}

void Adhoc::sendEndPacket() {

	AdhocDataPacket packet;
	packet.type = END_PACKET;
	if (server_) {
		adhoc::PSPelement* psp = adhoc::server::getClientList().at( 0 );
		while (adhoc::server::send(psp, sizeof(AdhocDataPacket), &packet) != 0);
	} else {
		while (adhoc::client::send(sizeof(AdhocDataPacket), &packet) != 0);
	}
	
}

void Adhoc::waitForReplay() {

	state_ = (server_) ? SERVER_WAIT_FOR_REPLAY : CLIENT_WAIT_FOR_REPLAY;
}

bool Adhoc::getServer() {

	return server_;
}

void Adhoc::replay() {

	if (!sendReplay_) {
		AdhocDataPacket packet;
		packet.type = REPLAY_PACKET;
		if (server_) {
			adhoc::PSPelement* psp = adhoc::server::getClientList().at( 0 );
			while (adhoc::server::send(psp, sizeof(AdhocDataPacket), &packet) != 0);
		} else {
			while (adhoc::client::send(sizeof(AdhocDataPacket), &packet) != 0);
		}
		sendReplay_ = true;
	}
}


void Adhoc::update() {

	if (server_) {

		if ( state_ == SERVER_WAIT_FOR_PLAYER ) {

			if (adhoc::server::getClientList().size() > 0) {
				serverStartGame_();
				state_ = SERVER_IN_GAME;
			}
		
		} else if (state_ == SERVER_IN_GAME) {

			// check if opponent is connected
			if (adhoc::server::getClientList().size() == 0) {
				serverDisconnect();
				quit_();
				state_ = NOTHING;
				return;
			}
				

			AdhocDataPacket packet;
			packet.type = PERSO_POS_PACKET;
			sprintf(packet.data, "%i,%i,%i,%i,%f,%f",
				game_->getHeroLife(),
				(int)game_->getHeroPos().x,
				(int)game_->getHeroPos().y,
				game_->getHero()->getArme()->getID(),
				game_->getHeroAngle(),
				game_->getHero()->getWalkFrame()
			);

			adhoc::PSPelement* psp = adhoc::server::getClientList().at( 0 );
			adhoc::server::send(psp, sizeof(AdhocDataPacket), &packet);
			
			
		} else if (state_ == SERVER_WAIT_FOR_REPLAY) {

			// check if opponent is connected
			if (adhoc::server::getClientList().size() == 0) {
				serverDisconnect();
				quit_();
				state_ = NOTHING;
				return;
			}

			while (Game::adhocMsg.size() > 0) {
				if (Game::adhocMsg.at(0).type == REPLAY_PACKET) {
					replay_ = true;
				}
				Game::adhocMsg.erase(Game::adhocMsg.begin());
			}

			if (replay_ && sendReplay_) {
				sendReplay_ = false;
				replay_ = false;
				state_ = (server_) ? SERVER_WAIT_FOR_PLAYER : CLIENT_WAIT_FOR_GAME;
			}
		}
		
	} else {
		
		
		if (state_ == CLIENT_WAIT_FOR_GAME) { // En attente de démarrage de la partie

			if (adhoc::client::connectionState() == adhoc::CLIENT_STATE_CONNECTED) {
				clientStartGame_();
				state_ = CLIENT_IN_GAME;
			}
			
		} else if (state_ == CLIENT_IN_GAME) {

			// check if opponent is connected
			if (adhoc::client::connectionState() == adhoc::CLIENT_STATE_DISCONNECTED) {
				clientDisconnect();
				quit_();
				state_ = NOTHING;
				return;
			}
			
		

			AdhocDataPacket packet;
			packet.type = PERSO_POS_PACKET;
			sprintf(packet.data, "%i,%i,%i,%i,%f,%f",
				game_->getHeroLife(),
				(int)game_->getHeroPos().x,
				(int)game_->getHeroPos().y,
				game_->getHero()->getArme()->getID(),
				game_->getHeroAngle(),
				game_->getHero()->getWalkFrame()
			);

			adhoc::client::send(sizeof(AdhocDataPacket), &packet);
			
			
		} else if (state_ == CLIENT_WAIT_FOR_REPLAY) {

			// check if opponent is connected
			if (adhoc::client::connectionState() == adhoc::CLIENT_STATE_DISCONNECTED) {
				clientDisconnect();
				quit_();
				state_ = NOTHING;
				return;
			}

			while (Game::adhocMsg.size() > 0) {
				if (Game::adhocMsg.at(0).type == REPLAY_PACKET) {
					replay_ = true;
				}
				Game::adhocMsg.erase(Game::adhocMsg.begin());
			}

			if (replay_ && sendReplay_) {
				sendReplay_ = false;
				replay_ = false;
				state_ = (server_) ? SERVER_WAIT_FOR_PLAYER : CLIENT_WAIT_FOR_GAME;
			}
			
		}
	}
}

void Adhoc::draw() {

	//~ if (state_ != NOTHING) {
		//~ if (server_) {
			//~ pspDebugScreenSetXY(0,10);
			//~ pspDebugScreenPrintf("[Server]\n");
			//~ pspDebugScreenPrintf("n : %i\n", n);
			//~ pspDebugScreenPrintf("msg : %s", msg );
			//~ 
		//~ } else {
			//~ pspDebugScreenSetXY(0,10);
			//~ pspDebugScreenPrintf("[Client]\n");
		//~ }
	//~ }
}






