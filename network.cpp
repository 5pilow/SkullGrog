#include "network.hpp"

#include <iostream>
#include <string>

#define PHP_PATH "http://gorglucks.free.fr/Online/Skullgrog/vIU_Io-trertyu-_iolZ_FRr_fa_GFD/Neo_-67_xXzU_Ulale_-GtXx.php"
#define KEY_NAME    "Neo_-_Key"
#define KEY "R-55__6o_FoDo"
#define PSEUDO_NAME "Neo_-_Pseudo"
#define SCORE_NAME  "Neo_-_Score"


Network::Network() {

	page_ = 1;
	rank_ = -1;
	connectionDialog_ = false;
	
	for (int i = 0; i < 20; i++) {
		scores_[i] = "---:---";
	}

}

Network::~Network() {}

void Network::internetConnection() {

	pgeNetInit();
	pgeUtilsNetDialogInit();
	
	connectionDialog_ = true;
}

void Network::internetDeconnection() {

	//~ pgeNetDisconnect();
	pgeNetShutdown();
}


bool Network::dialog() {
	
	return dialog_;
}
bool Network::isConnected() {

	return pgeNetIsConnected();
}

void Network::submitScore(int score, char* pseudo) {

	if (!isConnected()) return;
	
	char scoreStr[100];
	sprintf(scoreStr, "%s=%s&%s=%i&%s=%s", KEY_NAME,KEY, SCORE_NAME,score, PSEUDO_NAME,pseudo);
	pgeNetPostForm(PHP_PATH,scoreStr, data_, 500);
	sscanf(data_, "%i", &rank_);
}

bool Network::getScores() {

	if (!isConnected()) return false;

	
	char scoreStr[100];
	sprintf(scoreStr, "%s=%s&view=%i", KEY_NAME,KEY,page_);
	pgeNetPostForm(PHP_PATH,scoreStr, data_, 500);

	// extraction des scores à partir de data_
	std::string data(data_);
	
	
	if (data.size() < 1 || data[0] == '0') return false; // pas de scores
	
	for (int i = 0, pos = 1; i < 20; i++, pos++) {
		scores_[i].clear();
		while (data[pos] != ',')
			scores_[i].push_back(data[pos++]);
	}
	return true;
}

char* Network::getScores(int score) {
	
	return (char*)scores_[score].c_str();
}

int Network::getRank() {
	
	return rank_;
}

char* Network::getData() {
	
	return data_;	
}

int Network::getPage() {
	
	return page_;	
}

void Network::setNextAction(void f()) {

	nextAction_ = f;
}	

void Network::changePage(int change) {
	
	page_ += change;
	bool ok = getScores();
	if (!ok)
		page_ -= change;
}

void Network::update() {

	
}

void Network::draw() {

	dialog_ = false;
	
	if (connectionDialog_) {
		
		int result = pgeUtilsNetDialogUpdate();
		dialog_ = true;
		if(result != -1) {
			connectionDialog_ = false;
			if (nextAction_) {
				nextAction_();
				nextAction_ = NULL;
			}
		}
	}
}
