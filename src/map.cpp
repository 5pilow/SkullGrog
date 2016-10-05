#include "map.hpp"
#include "sprite.hpp"
#include <pspkernel.h>

#include <stdlib.h>
#include <string.h>
#include <string>

// Textures de la map
#include "R/map1.c"
#include "R/map2.c"
#include "R/map3.c"
#include "R/map4.c"
#include "R/map5.c"
#include "R/map6.c"
#include "R/map7.c"
#include "R/map8.c"
#include "R/map9.c"

#define abs(x) pgeMathAbs(x)

#define ZONE_X 500
#define ZONE_Y 300

Map::Map() {

	Plateau plafon    = {-20,-20,1520,50};   
	Plateau sol       = {-20,850,1520,920};
	Plateau murGauche = {-20,50,50,920};
	Plateau murDroit = {1450,-20,1520,920};
	Plateau plateauHaut1 = {50,200,300,250};
	Plateau plateauHaut2 = {400,200,1000,250};
	Plateau plateauHaut3 = {1150,200,1450,250};
	Plateau plateauMilieu1 = {150,400,750,450};
	Plateau plateauMilieu2 = {850,400,1450,450};
	Plateau plateauBas1 = {50,600,550,650};
	Plateau plateauBas2 = {700,600,900,650};
	Plateau plateauBas3 = {1050,650,1300,700};
	Plateau murBas1 = {500,600,550,700};
	Plateau murBas2 = {1050,650,1100,850};
	Plateau murBas3 = {850,400,900,650};
	Plateau caisse1 = {950,750,1100,900};
	Plateau caisse2 = {900,800,1000,900};
	Plateau murHaut = {400,200,450,450};
	
	addPlateau(plafon);
	addPlateau(sol);
	addPlateau(murGauche);
	addPlateau(murDroit);
	addPlateau(plateauHaut1);
	addPlateau(plateauHaut2);
	addPlateau(plateauHaut3);
	addPlateau(plateauMilieu1);
	addPlateau(plateauMilieu2);
	addPlateau(plateauBas1);
	addPlateau(plateauBas2);
	addPlateau(plateauBas3);
	addPlateau(murBas1);
	addPlateau(murBas2);
	addPlateau(murBas3);
	addPlateau(caisse1);
	addPlateau(caisse2);
	addPlateau(murHaut);
	
	
	///-----///

	vramTex_ = pgeTextureCreate(ZONE_X,ZONE_Y,PGE_PIXEL_FORMAT_8888,PGE_VRAM);
	if (!vramTex_) sceKernelExitGame();
	
	int px[9] = {0,500,1000,0,500,1000,0,500,1000};
	int py[9] = {0,0,0,300,300,300,600,600,600};
	
	for (int i = 0; i < 9; i++) {
		Zone zone = {px[i],py[i],ZONE_X,ZONE_Y,NULL,0};
		map_.push_back(zone);
	}
	 
	vramTexZone_ = 0;
	drawVramTex_ = 60;
	numDraw_ = 0;

	camera_.x = 0;
	camera_.y = 0;
}

void Map::loadTextures() {
	
	unsigned int sizes[9] = {size_R_map1, size_R_map2, size_R_map3, size_R_map4, size_R_map5, size_R_map6, size_R_map7, size_R_map8, size_R_map9};
	unsigned char* maps[9] = {R_map1, R_map2, R_map3, R_map4, R_map5, R_map6, R_map7, R_map8, R_map9};
	
	for (int i = 0; i < 9; i++) {
		pgeTexture* tex = pgeTextureLoadMemory(maps[i],sizes[i],PGE_RAM,0);
		if (!tex) sceKernelExitGame();
		map_[i].tex = tex;
	}
}

void Map::reset() {

	/// destruction des textures
	for (int i = 0; i < 9; i++) {
		pgeTextureDestroy(map_[i].tex);
	}
	loadTextures();

	///suppression des waiting particles
	waitingParticles_.clear();
}

void Map::setCamera(ScePspFVector2 camera) {
	
	camera_ = camera;
}

void Map::increaseParticleZone(int zone) {

	map_[zone].particles ++;
}

bool Map::getTile(short x, short y) {

	return false;
}

Tile Map::getTile(float x, float y) {
	
	Tile tile = {x/50-1,y/50-1};
	return tile;
}

int Map::getMapTile(short x,short y) {
	
	return GetMap(x,y);
}

int Map::getZone(float x,float y) {
	int zone = -1;
	for (int i = 0; i < 9; i++) {
		pgeRect2 rect = {map_[i].x,map_[i].y,map_[i].x+map_[i].w,map_[i].y+map_[i].h};
		if (pgeMathRectTestPoint(&rect, x,y)) {
			zone = i;
		}
	}
	return zone;
}

std::vector<int> Map::getZones(float x,float y,float w,float h) {
	std::vector<int> zones;
	for (int i = 0; i < 9; i++) {
		pgeRect2 rect = {map_[i].x,map_[i].y,map_[i].x+map_[i].w,map_[i].y+map_[i].h};
		pgeRect2 carre = {x-w/2,y-h/2,x+w/2,y+h/2};
		if (pgeMathRectIntersect(&rect, &carre)) {
			zones.push_back(i);
		}
	}
	return zones;
}

int Map::getZoneWithMostParticles() {
	
	int zone = 0, particles = map_[0].particles;
	for (int i = 1; i < 9; i++) {
		if (map_[i].particles > particles) {
			particles = map_[i].particles;
			zone = i;
		}
	}
	return zone;
}	

float Map::collideX(float x,float y) {

	for (int i = 0; i < (int)plateaux_.size(); i++)
		if (pgeMathRectTestPoint(&plateaux_[i], x,y))
			return true;
	return false;
}

float Map::collideY(float x,float y,float dy) {

	for (int i = 0; i < (int)plateaux_.size(); i++) {
		
		if (pgeMathRectTestPoint(&plateaux_[i], x,y)) {

			if (dy > 0) {
				return plateaux_[i].y1 - 0.1f;
			} else if (dy < 0) {
				return plateaux_[i].y2;
			}
		}
	}
	return y;
}

void Map::addPlateau(Plateau plateau) {
	
	plateaux_.push_back(plateau);
}

void Map::drawParticleFond(Sprite* sprite, float x, float y, float angle, char alpha) {

	pgeGfxRenderToTexture(vramTex_);
	sprite->draw2DNoCamera(x - map_[vramTexZone_].x,y - map_[vramTexZone_].y, angle,alpha,false,false);
	pgeGfxRenderToScreen();
}

void Map::addParticleFond(Sprite* sprite, float x, float y, float angle, char alpha) {

	if (collideX(x,y)) return;
	
	int zone = getZone(x,y);
	std::vector<int> zones = getZones(x,y,sprite->getWidth(),sprite->getHeight());
	
	for (int i = 0; i < (int)zones.size(); i++) {
		WaitingParticle wp = {zones[i], x,y, angle, alpha, sprite};
		waitingParticles_.push_back(wp);
		map_[zones[i]].particles++;
	}
}

void Map::draw() {

	
	/// --- ///
	drawVramTex_--;

	if (waitingParticles_.size() > 20 && drawVramTex_ <= 0) {

		// on selectionne la zone avec le plus de particules
		vramTexZone_ = getZoneWithMostParticles();

		// on dessine sur la texture en vram l'image en ram correspondante
		pgeGfxRenderToTexture(vramTex_);
		pgeTextureActivate(map_[vramTexZone_].tex);
		pgeGfxDrawTextureEasy(map_[vramTexZone_].tex,0,0,0,255);
		pgeGfxRenderToScreen();
		
		// on affiche toutes les particules de cette zone sur la texture en vram
		for (int i = 0; i < (int) waitingParticles_.size(); i++) {
				
			if (waitingParticles_[i].zone == vramTexZone_) {
				drawParticleFond( waitingParticles_[i].sprite,  waitingParticles_[i].x, waitingParticles_[i].y, waitingParticles_[i].angle, waitingParticles_[i].alpha );
				waitingParticles_.erase(waitingParticles_.begin() + i);
				i--;
			}
		}

		// enfin on copy l'image en vram sur l'image en ram
		pgeTexture* tex = map_[vramTexZone_].tex;
		sceGuCopyImage(GU_PSM_8888,0,0,500,300,vramTex_->textureWidth,vramTex_->data,  0,0,tex->textureWidth,tex->data);
		map_[vramTexZone_].particles = 0;
		numDraw_++;

		drawVramTex_ = 20;
	}
	
	/// --- ///
	
	// draw background
	for (int i = 0; i < 9; i++) {
		
		pgeTextureActivate(map_[i].tex);
		pgeGfxDrawTextureEasy(map_[i].tex,map_[i].x-(int)camera_.x,map_[i].y-(int)camera_.y, 0,255);
	}
	


	/*
	// draw walls
	for (int i = 0; i < (int)plateaux_.size(); i++) {
		pgeRect rect = {plateaux_[i].x1 -  (int)camera_.x, plateaux_[i].y1 - (int)camera_.y, plateaux_[i].x2 -plateaux_[i].x1,  plateaux_[i].y2 -plateaux_[i].y1};
		pgeGfxDrawRect(&rect, 0x55000000, 0 );
		pgeGfxDrawRectOutline(&rect, 0xffff0000, 0 );
	}
	*/
	
	/*
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 30; j++) {
			pgeRect rect = {j*50 - (int)camera_.x, i*50 - (int)camera_.y, 50,50};
			pgeGfxDrawRect(&rect, grille_[i][j] == 0 ? 0x5500ff00 : 0x550000ff, 0);
		}	
	*/	
	
	// draw static particles
	for (int i = 0; i < (int) waitingParticles_.size(); i++) {
		
		waitingParticles_[i].sprite->draw2D(waitingParticles_[i].x, waitingParticles_[i].y, waitingParticles_[i].angle, waitingParticles_[i].alpha, false, false);
	}
	
}

std::vector<Tile>& Map::findPath(Tile start, Tile end) {
	
	chemin_.clear();
	
	// Create a start state
	MapSearchNode nodeStart;
	nodeStart.x = start.x;
	nodeStart.y = start.y; 

	// Define the goal state
	MapSearchNode nodeEnd;
	nodeEnd.x = end.x;						
	nodeEnd.y = end.y; 
	
	// Set Start and goal states
	
	astarsearch_.SetStartAndGoalStates( nodeStart, nodeEnd );

	unsigned int SearchState;
	unsigned int SearchSteps = 0;
	
	
	do {
		SearchState = astarsearch_.SearchStep();

		SearchSteps++;

	} while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );
	

	if ( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED ) {
		
		MapSearchNode *node = astarsearch_.GetSolutionStart();
		
		Tile tile = {node->x, node->y};
		chemin_.push_back(tile);
		
		while (true) {
			node = astarsearch_.GetSolutionNext();
			if (node == NULL) break;
			tile.x = node->x;
			tile.y = node->y;
			chemin_.push_back(tile);
		}

		// Once you're done with the solution you can free the nodes up
		astarsearch_.FreeSolutionNodes();

	} else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED )  {
		
	}

	astarsearch_.EnsureMemoryFreed();
	
	
	return chemin_;
}

