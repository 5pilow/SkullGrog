#include "menu.hpp"

#include <iostream>
#include <string>
#include "mp3.h"
#include "options.h"

#define print pspDebugScreenPrintf
#define sText intraFontMeasureText

#define SCROLL 385;

static void printCenter(intraFont* font,float y,char* text) {

	float x = 240 - sText(font,text)/2;
	intraFontPrint(font,x,y,text);
}

Menu::Menu(intraFont* font, intraFont* font2, pgeTexture* logo, Stats* stats, Network* network, Adhoc* adhoc) {

	font_ = font;
	font2_ = font2;
	dialog_ = 0;
	logo_ = logo;
	stats_ = stats;
	network_ = network;
	adhoc_ = adhoc;

	scroll_ = SCROLL;

}

Menu::~Menu() {}

void Menu::addDialog(char* title, DialogType type) {

	int x = (type == MAIN_MENU) ? 120 : 240;
	int y = (type == MAIN_MENU) ? 200 : 136;

	int sizeX = type == ABOUT || type == ADHOC_CLIENT || type == ADHOC_SERVER || type == WEAPONS ? 450 : 350;
	int sizeY = type == WEAPONS ? 220 : 150;
	std::vector<Item> items;
	Dialog dialog = {title, items, 0, 0, 0, type, x,y, sizeX,sizeY};
	dialogs_.push_back(dialog);
}

void Menu::addItem(int dialog, char* text, void f() ) {

	Item item;
	item.text = text;
	item.f = f;
	dialogs_[dialog].items.push_back(item);
}

void Menu::setDialog(int dialog) {

	dialog_ = dialog;
}

int Menu::getDialog() {

	return dialog_;
}


void Menu::update() {

	if (dialog_ == -1) return;

	dialogs_[dialog_].time++;

	if (dialogs_[dialog_].pos != dialogs_[dialog_].select) {
		dialogs_[dialog_].pos += (dialogs_[dialog_].select - dialogs_[dialog_].pos)/10;
	}	

	if (pgeControlsPressed(PGE_CTRL_UP)) {
		dialogs_[dialog_].select--;
		if (dialogs_[dialog_].select < 0) {
			dialogs_[dialog_].select = 0;
		}
		scroll_ = SCROLL;
	}
	if (pgeControlsPressed(PGE_CTRL_DOWN)) {
		dialogs_[dialog_].select++;
		if (dialogs_[dialog_].select > dialogs_[dialog_].items.size() - 1) {
			dialogs_[dialog_].select = dialogs_[dialog_].items.size() - 1;
		}
		scroll_ = SCROLL;
	}

	if (pgeControlsPressed(PGE_CTRL_CROSS) ) {
		if (dialog_ != 8 && dialog_ != 14) {
			if (dialogs_[dialog_].items[dialogs_[dialog_].select].f) {
				dialogs_[dialog_].items[dialogs_[dialog_].select].f();
			}
		} else if (dialog_ == 14) {
			if (dialogs_[dialog_].select == 2) {
				char pseudo[128];
				stats_->getPseudo(pseudo);
				char newPseudo[128];
				sprintf(newPseudo, Utils::osk("Enter a new Pseudo",pseudo));
				if (strlen(newPseudo) > 0) {
					stats_->setPseudo(newPseudo);
					sprintf(dialogs_[14].items[2].text , "Pseudo : %s", newPseudo);
				}
			}
		} else {
			// demande de connection au serveur
			adhoc_->requestConnection(dialogs_[dialog_].select);
		}
	}

	if (pgeControlsPressed(PGE_CTRL_LEFT) ) {
		if (dialog_ == 14) {
			if (dialogs_[dialog_].select == 0 && getVolume() > 0) {
				setVolume(getVolume()-5);
				sprintf(dialogs_[dialog_].items[dialogs_[dialog_].select].text, "< Music Volume : %i >",getVolume());
			} else if (dialogs_[dialog_].select == 1) {
				stats_->setControls(stats_->getControls() == 0 ? 1 : 0);
				sprintf(dialogs_[dialog_].items[1].text, "< Controls : Mode %i >",stats_->getControls()+1);
			}
		}
	}
	if (pgeControlsPressed(PGE_CTRL_RIGHT) ) {
		if (dialog_ == 14) {
			if (dialogs_[dialog_].select == 0 && getVolume() < 100) {
				setVolume(getVolume()+5);
				sprintf(dialogs_[dialog_].items[0].text, "< Music Volume : %i >",getVolume());
			} else if (dialogs_[dialog_].select == 1) {
				stats_->setControls(stats_->getControls() == 0 ? 1 : 0);
				sprintf(dialogs_[dialog_].items[1].text, "< Controls : Mode %i >",stats_->getControls()+1);
			}
		}
	}
	
	if (pgeControlsPressed(PGE_CTRL_CIRCLE)) {
		if (dialogs_[dialog_].type == WEAPONS || dialogs_[dialog_].type == SCORES || dialog_ == 6 || dialog_ == 9 || dialog_ == 14) {
			dialog_ = 0;			
		} else if (dialog_ == 7 || dialog_ == 8) {
			dialog_ = 6;
			adhoc_->quit();
		} else if (dialog_ == 0) {
			dialog_ = 11;			
		}
	}
	if (dialogs_[dialog_].type == SCORES) {
		if (pgeControlsPressed(PGE_CTRL_LTRIGGER)) {
			network_->changePage(-1);
			updateScoreMenu();
			dialogs_[dialog_].select = 0;
		} else if (pgeControlsPressed(PGE_CTRL_RTRIGGER)) {
			network_->changePage(1);
			updateScoreMenu();
			dialogs_[dialog_].select = 0;
		}
	}

	if (dialog_ == 8) {
		updateAdhocClientMenu();

	}
	if (dialog_ == 7) { // Serveur
		updateAdhocServerMenu();
	}
	
}

void Menu::updateWeaponsMenu() {
	
	dialogs_[4].items.clear();
	
	for (int i = 0; i < 10; i++) {
		
		if (stats_->getWeapon(i).unlocked == 1)
			addItem(4, stats_->getWeapon(i).name, NULL);
		else
			addItem(4, "???", NULL);
	}
}

void Menu::updateScoreMenu() {
	
	dialogs_[5].items.clear();
	
	for (int i = 0; i < 20; i++)
		addItem(5, network_->getScores(i), NULL);
}

void Menu::updateAdhocClientMenu() {

	dialogs_[8].items.clear();
	for (int i = 0; i < adhoc::client::getServerList().size(); i++)
		addItem(8, adhoc::client::getServerList().at(i)->hellodata, NULL);
}

void Menu::updateAdhocServerMenu() {

	dialogs_[7].items.clear();
	for (int i = 0; i < adhoc::server::getClientList().size(); i++)
		addItem(7, adhoc::server::getClientList().at(i)->hellodata, NULL);
}



void Menu::draw() {

	if (dialog_ == -1) return;

	// Title
	if (dialogs_[dialog_].type == MAIN_MENU) {
		pgeTextureActivate(logo_);
		pgeGfxDrawTextureEasy(logo_,65,-15,0,255);
	} else if (dialogs_[dialog_].type == SCORES) {
	
	} else {
		Utils::drawBox((int)dialogs_[dialog_].x, (int)dialogs_[dialog_].y, (int)dialogs_[dialog_].sx,(int)dialogs_[dialog_].sy);
		intraFontSetStyle(font2_, 1.4f, 0xffffffff, 0x00000000, 0);
		intraFontPrintf(font2_, dialogs_[dialog_].x - sText(font2_,dialogs_[dialog_].title)/2, dialogs_[dialog_].y - 50, "%s",dialogs_[dialog_].title);
	}
	
	
	if (dialogs_[dialog_].type == SCORES) {

		for (int i = (int)dialogs_[dialog_].pos-3; i < dialogs_[dialog_].select+5; i++) {

			if (i<0 || i >= dialogs_[dialog_].items.size()) continue;
			
			unsigned int color = (dialogs_[dialog_].select == i) ? 0xff0000ff : 0x00ffffff;
			intraFontSetStyle(font_, 1.1f, 0xffffffff, color, 0);

			float y = dialogs_[dialog_].y + 15 + (i-dialogs_[dialog_].pos)*27;
			
			std::string pseudo;
			int j;
			for (j = 0; j < strlen(dialogs_[dialog_].items[i].text); j++){
				if (dialogs_[dialog_].items[i].text[j] == ':') break;
				pseudo.push_back(dialogs_[dialog_].items[i].text[j]);
			}
			std::string score;
			for (j = ++j; j < strlen(dialogs_[dialog_].items[i].text); j++){
				score.push_back(dialogs_[dialog_].items[i].text[j]);
			}
			const char* scoreStr = Utils::virgulesMilliers((char*)score.c_str());

			char rankStr[4];
			sprintf(rankStr, "%i.", (i+1) + 20*(network_->getPage()-1));
			intraFontPrint(font_, 20 - sText(font_,rankStr)/2, y, rankStr);
			intraFontPrintf(font_, 50, y, "%s",pseudo.c_str());
			intraFontPrintf(font_, 470 - sText(font_,scoreStr), y, "%s",scoreStr);
		}
		
		pgeRect rect = {0,0,480,30};
		pgeGfxDrawRect(&rect, 0xff000000, 0);
		rect.y += 30;
		pgeGfxDrawRectGrad(&rect, 0xff000000,0,0,0xff000000, 0);
		rect.y = 220;
		pgeGfxDrawRectGrad(&rect, 0,0xff000000,0xff000000,0, 0);
		rect.y += 30;
		pgeGfxDrawRect(&rect, 0xff000000, 0);
		
		// Title
		intraFontSetStyle(font2_, 1.4f, 0xffffffff, 0x00000000, 0);
		intraFontPrintf(font2_, dialogs_[dialog_].x - sText(font2_,dialogs_[dialog_].title)/2, 30, "%s",dialogs_[dialog_].title);
		
		intraFontSetStyle(font_, 0.7f, 0xffffffff, 0, 0);
		intraFontPrintf(font_, 10, 265, "Page %i", network_->getPage());
		
	} else if (dialogs_[dialog_].type == ABOUT) {

		intraFontSetStyle(font_, 0.8f, 0xffffffff, 0, 0);
		float y = 95, inc = 18;
		printCenter(font_, y+=inc, "Program and graphics by gorglucks");
		printCenter(font_, y+=inc, "Zombies screams by Firness");
		printCenter(font_, y+=inc, "Music : The Two R - Scared");
		printCenter(font_, y+=inc, "Using IntraFont by BenHur,");
		printCenter(font_, y+=inc, "PGE by InsertWittyName and MK2k");
		printCenter(font_, y+=inc, "Thanks to Firness and Rineus for betatesting");
		intraFontSetStyle(font_, 0.7f, 0xaaffffff, 0, 0);
		intraFontPrint(font_, 450, 268, "v1.1");
		
	} else if (dialogs_[dialog_].type == WEAPONS) {

		for (int i = (int)dialogs_[dialog_].pos-2; i < (int)dialogs_[dialog_].select+3; i++) {

			if (i<0 || i >= dialogs_[dialog_].items.size()) continue;
			
			unsigned int color = (dialogs_[dialog_].select == i) ? 0xff0000ff : 0x00ffffff;
			int alpha = (dialogs_[dialog_].select != i) ? 255 - pgeMathAbs(i - dialogs_[dialog_].pos)*100 : 255;
			if (alpha > 255) alpha = 255; if (alpha < 0) alpha = 0;
			intraFontSetStyle(font_, 1.2f, pgeGfxCreateColor(255,255,255,alpha), color, 0);

			float x = 110 - sText(font_,dialogs_[dialog_].items[i].text)/2;
			float y = dialogs_[dialog_].y + 20 + (i-dialogs_[dialog_].pos)*27;
				
			intraFontPrintf(font_, x, y, "%s",dialogs_[dialog_].items[i].text);
			
		}
		if (stats_->getWeapon(dialogs_[dialog_].select).unlocked == 1) {

			// image
			stats_->getArme(dialogs_[dialog_].select)->getSprite()->draw2DNoCamera(350,130,0,255,false,false);

			// power
			intraFontSetStyle(font_, 0.8f, 0xffffffff, 0x00ffffff, 0);
			intraFontPrint(font_, 250,180, "Power :");
			for( int j = 0; j < stats_->getWeapon(dialogs_[dialog_].select).power; j++)
				pgeGfxDrawStar(325 + j*20, 175, 4, 10, 5, 0xaa00ffff, GU_PI);

			// speed
			intraFontPrint(font_, 250,200, "Speed :");
			for( int j = 0; j < stats_->getWeapon(dialogs_[dialog_].select).speed; j++)
				pgeGfxDrawStar(325 + j*20, 195, 4, 10, 5, 0xaa00ffff, GU_PI);

			// unlocking
			intraFontPrint(font_, 212,220, "Unblocking :");
			intraFontSetStyle(font_, 0.8f, 0xffffffff, 0x00ffffff, INTRAFONT_SCROLL_SEESAW);
			if (intraFontMeasureText(font_,stats_->getWeapon(dialogs_[dialog_].select).unlocking) < 130) scroll_ = 320;
			scroll_ = intraFontPrintColumn(font_, scroll_, 220, 130, stats_->getWeapon(dialogs_[dialog_].select).unlocking);
			
		} else {

			intraFontSetStyle(font_, 1.2f, 0xffffffff, 0x00ffffff, 0);
			intraFontPrint(font_, 300,150, "?");
		}

	} else {

		for (int i = (int)dialogs_[dialog_].pos-2; i < (int)dialogs_[dialog_].select+3; i++) {

			if (i<0 || i >= dialogs_[dialog_].items.size()) continue;
			
			unsigned int color = (dialogs_[dialog_].select == i) ? 0xff0000ff : 0x00ffffff;
			
			float size = 1.2f;
			if (dialogs_[dialog_].type == MAIN_MENU)
				size += (0.4f - pgeMathAbs(i - dialogs_[dialog_].pos)/3);

			int alpha = (dialogs_[dialog_].select != i) ? 255 - pgeMathAbs(i - dialogs_[dialog_].pos)*100 : 255;
			if (alpha > 255) alpha = 255; if (alpha < 0) alpha = 0;
			intraFontSetStyle(font_, size, pgeGfxCreateColor(255,255,255,alpha), color, 0);

			float x = dialogs_[dialog_].x - sText(font_,dialogs_[dialog_].items[i].text)/2;
			
			float y;
			if (dialogs_[dialog_].type == MAIN_MENU)
				y = dialogs_[dialog_].y + 20 + (i-dialogs_[dialog_].pos)*(size*20);
			else
				y = dialogs_[dialog_].y + 20 + (i-dialogs_[dialog_].pos)*24;
				
			intraFontPrintf(font_, x, y, "%s",dialogs_[dialog_].items[i].text);
		}
		
		
	}
}
