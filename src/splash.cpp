#include "splash.hpp"

Splash::Splash(pgeTexture* tex1,pgeTexture* tex2,pgeTexture* tex3, pgeTexture* logoGorglucks, pgeTexture* logoScenery, intraFont* font) {

	logo1_ = tex1;
	logo2_ = tex2;
	logo3_ = tex3;
	logoGorglucks_ = logoGorglucks;
	logoScenery_ = logoScenery;
	font_ = font;

	state_ = 1;
	time_ = 0;
	alpha_ = 0;
	dAlpha_ = 1.5f;
	angle_ = 0;
}

Splash::~Splash() {}

bool Splash::isFinish() {
	
	return state_ == 4;
}

void Splash::update() {

	if (state_ == 1) {
		time_++;
		if (time_ > 150) {
			time_ = 0;
			state_++;
			alpha_ = 0;
		}
		alpha_ += dAlpha_;
		
	} else if (state_ == 2) {
		angle_++;
		alpha_ += dAlpha_*4;
		if (alpha_ >= 255) {
			alpha_ = 255;
			dAlpha_ *=-1.5f;
		} else if (alpha_ <= 0) {
			state_++;
			alpha_ = 0;
			time_ = 0;
			dAlpha_ *=-1;
		}
	} else if (state_ == 3) {
		time_++;
		if (time_ > 200) {
			time_ = 0;
			state_++;
			alpha_ = 0;
		}
		alpha_ += dAlpha_;


	}
	
}

void Splash::draw() {

	pgeGfxStartDrawing();
	pgeGfxClearScreen(0);

	if (state_ == 1) {

		pgeTextureActivate(logoGorglucks_);
		int alpha = (int)alpha_+100;
		pgeGfxDrawTextureEasy(logoGorglucks_,240-150,100,0,alpha);
		intraFontSetStyle(font_, 1.4f, pgeGfxCreateColor(255,255,255,alpha_), 0x00ffffff, 0);
		intraFontPrint(font_, 300, 200, "presents");
		
	} else if (state_ == 2) {

		
		pgeTextureActivate(logo1_);
		pgeRect rect = {240-110,0,220,220};
		pgeRect rect2 = {0,0,300,300};
		pgeGfxDrawTexture(&rect,&rect2,angle_/20,alpha_);
		pgeTextureActivate(logo2_);
		pgeRect rect3 = {240-102.5f,7.5f,205,205};
		pgeRect rect4 = {0,0,280,280};
		pgeGfxDrawTexture(&rect3,&rect4,-angle_/40,alpha_);
		pgeTextureActivate(logo3_);
		pgeRect rect5 = {240-123/2,(220-123)/2,123,123};
		pgeRect rect6 = {0,0,168,168};
		pgeGfxDrawTexture(&rect5,&rect6,angle_/30,alpha_);
		
		intraFontSetStyle(font_, 1.0f, pgeGfxCreateColor(255,255,255,alpha_), 0x00ffffff, 0);
		intraFontPrint(font_, 240 - intraFontMeasureText(font_,"PSP-Hacks.com Homebrew Horror Show")/2, 250, "PSP-Hacks.com Homebrew Horror Show");
		
	} else if (state_ == 3) {
		pgeTextureActivate(logoScenery_);
		pgeGfxDrawTextureEasy(logoScenery_,0,0,0,alpha_);
	}
	
	pgeGfxEndDrawing();
	pgeGfxSwapBuffers(PGE_WAIT_VSYNC);

}
