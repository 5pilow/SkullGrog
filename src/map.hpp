#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <vector>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>
#include "findpath.hpp"


class Sprite;

typedef pgeRect2 Plateau;

typedef ScePspSVector2 Tile;


typedef struct Zone {
		
	int x,y,w,h;
	pgeTexture* tex;
	int particles;
	
} Zone;

typedef struct WaitingParticle {
	
	int zone;
	float x,y, angle, alpha;
	Sprite* sprite;
	
} WaitingParticle;



class Map {
	
	private :
	
		
		std::vector<Plateau> plateaux_;

		std::vector<WaitingParticle> waitingParticles_;
		pgeTexture* vramTex_;
		std::vector<Zone> map_;
		int vramTexZone_;
		int drawVramTex_;
		int numDraw_;

		ScePspFVector2 camera_;
		
		std::vector<Tile> chemin_;
		
		// Astar
		AStarSearch<MapSearchNode> astarsearch_;
		
	
	public :
	
		Map();
		~Map();

		void reset();
		
		void loadTextures();
		
		void addPlateau(Plateau plateau);
		
		float collideX(float x,float y);
		float collideY(float x,float y,float dy);
		
		bool getTile(short x, short y);
		Tile getTile(float x, float y);
		int getMapTile(short x,short y);

		void setCamera(ScePspFVector2 camera);
		
		int getZoneWithMostParticles();
		int getZone(float x,float y);
		std::vector<int> getZones(float x,float y,float w,float h);
		void increaseParticleZone(int zone);
		int getNumParticles();
		void addParticleFond(Sprite* sprite,float x, float y, float angle, char alpha);
		void drawParticleFond(Sprite* sprite, float x, float y, float angle, char alpha);
		
		void draw();

		std::vector<Tile>& findPath(Tile start, Tile end);

};

#else

class Map;

#endif
