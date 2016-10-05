#ifndef UTILS_H
#define UTILS_H

#include <pge/pgeMath.h>
#include <pge/pgeSystem.h>
#include <pge/pgeControls.h>
#include <pge/pgeUtils.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include "sprite.hpp"

class Utils {

	public :

		static Sprite* boxCorner;
		static Sprite* boxEdge1;
		static Sprite* boxEdge2;
	
		static float normalize(float n);
		
		static char* strTime(int time);
		
		static char* virgulesMilliers(int n);
		static char* virgulesMilliers(char* n);

		static void drawBox(int x, int y, int w, int h);

		static char* osk(char* text, char* text2);

		static char* getPSPPseudo();

		static void debug(char* msg, ...);
	
};

#endif
