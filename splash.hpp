#ifndef SPLASH_H
#define SPLASH_H

#include <vector>
#include "pge/pgeTexture.h"
#include "pge/pgeGfx.h"
#include "intraFont/intraFont.h"
#include <pspdebug.h> 


class Splash {
	
	private :
	
		int state_;
		pgeTexture* logo1_;
		pgeTexture* logo2_;
		pgeTexture* logo3_;
		pgeTexture* logoGorglucks_;
		pgeTexture* logoScenery_;
		intraFont* font_;
		int time_;
		int alpha_;
		int dAlpha_;
		float angle_;
		
		
	
	public :
		
	
		Splash(pgeTexture* tex1,pgeTexture* tex2,pgeTexture* tex3, pgeTexture* logoGorglucks,pgeTexture* logoScenery, intraFont* font);
		~Splash();

		bool isFinish();
		void update();
		void draw();
	
	
};

#else 

class Splash;

#endif
