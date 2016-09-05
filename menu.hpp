#ifndef MENU_H
#define MENU_H

#include <vector>
#include <pge/pgeGfx.h>
#include "intraFont/intraFont.h"
#include "game.hpp"
#include "stats.hpp"
#include "network.hpp"
#include "adhoc.hpp"
#include <string>

enum DialogType {NORMAL, MAIN_MENU, WEAPONS, SCORES, ADHOC_SERVER, ADHOC_CLIENT, ABOUT};

typedef struct Item {

	char* text;
	void (*f)();

} Item;

typedef struct Dialog {

	char* title;
	std::vector<Item> items;
	int select;
	int time;
	float pos;
	DialogType type;
	float x, y;
	float sx, sy;

} Dialog;

class Menu {

	private :

		std::vector<Dialog> dialogs_;

		int dialog_;

		intraFont* font_;
		intraFont* font2_;

		pgeTexture* logo_;
		
		Stats* stats_;
		Network* network_;
		Adhoc* adhoc_;

		float scroll_;
		

	public :

		Menu(intraFont* font, intraFont* font2, pgeTexture* logo, Stats* stats, Network* network, Adhoc* adhoc);
		~Menu();
		
		void addDialog(char* title, DialogType type);
		void addItem(int dialog, char* text, void f() );
		
		void updateWeaponsMenu();
		void updateScoreMenu();
		void updateAdhocClientMenu();
		void updateAdhocServerMenu();

		void setDialog(int dialog);
		int getDialog();

		void update();
		
		void draw();
	

};

#else

class Menu;

#endif
