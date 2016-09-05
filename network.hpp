#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <pge/pgeNet.h>
#include <pge/pgeUtils.h>
#include <pge/pgeGfx.h>
#include <pspkernel.h>


class Network {

	private :

		bool connectionDialog_;
		bool dialog_;

		void (*nextAction_)();

		char data_[500];
		
		std::string scores_[20];
		int rank_;
		int page_;

		

	public :

		Network();
		~Network();

		void internetConnection();
		void internetDeconnection();

		bool dialog();
		bool isConnected();
		
		void setNextAction(void f());
		void submitScore(int score, char* pseudo);

		int getRank();
		bool getScores();
		char* getScores(int score);
		char* getData();
		int getPage();

		void changePage(int change);
		
		void update();

		void draw();

		

};

#else 

class Network;

#endif
