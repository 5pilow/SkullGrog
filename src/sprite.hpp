#ifndef SPRITE_H
#define SPRITE_H

#include <pge/pgeTexture.h>
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>

#include <pspgu.h>
#include <pspgum.h>
//~ #include <pspkernel.h>

//~ #include <vector>

class Sprite {
	
	private :
	
	
		pgeTexture* texture_;
	
		pgeRect rect_;
		
		float w_,h_;
		
		
		
	
	public :
		
		static ScePspFVector2 camera;
		static ScePspFVector2 scale;
		static unsigned int color;
		
	
		Sprite(pgeTexture* tex, int x, int y, int w, int h);
		~Sprite();
		
		static void setCamera(ScePspFVector2 newCam);
		static void setScale(float x, float y);
		static void setColor(unsigned int newColor);
		
		void setScale(float scale);
		
		int getWidth();
		int getHeight();
		
		unsigned int getPixel(int x, int y);
		
		pgeTexture* getTexture();
		
		void draw2D(float x, float y, float angle, float alpha, bool reverseX, bool reverseY);
		void draw2DNoCamera(float x, float y, float angle, float alpha, bool reverseX, bool reverseY);
		
		void draw3D(ScePspFVector3& pos, float angle, float alpha, bool reverseX, bool reverseY);
		
		
	
	
};

#else 

class Sprite;

#endif
