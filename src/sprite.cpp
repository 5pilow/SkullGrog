#include "sprite.hpp"

ScePspFVector2 Sprite::camera = {0,0};
ScePspFVector2 Sprite::scale = {1,1};
unsigned int Sprite::color = 0xffffffff;

Sprite::Sprite(pgeTexture* tex, int x, int y, int w, int h) {
	
	texture_ = tex;
	pgeRect rect = {x,y,w,h};
	rect_ = rect;
	w_ = w;
	h_ = h;
}

Sprite::~Sprite() {}

void Sprite::setCamera(ScePspFVector2 newCam) {
	
	camera.x = newCam.x;
	camera.y = newCam.y;
}

void Sprite::setScale(float x, float y) {
	
	scale.x = x;
	scale.y = y;
}

void Sprite::setScale(float scale) {
	
	w_ *= scale;
	h_ *= scale;	
}

int Sprite::getWidth() {
	
	return rect_.w;
}

int Sprite::getHeight() {
	
	return rect_.h;
}

unsigned int Sprite::getPixel(int x, int y) {
	
	return pgeTextureGetPixel(texture_,x,y);
}

pgeTexture* Sprite::getTexture() {
	
	return texture_;
}

void Sprite::setColor(unsigned int newColor) {
		
	color = newColor;
}


void Sprite::draw2DNoCamera(float x, float y, float angle, float alpha, bool reverseX, bool reverseY) {
	
	draw2D(x + camera.x, y + camera.y, angle, alpha, reverseX, reverseY);
}	

void Sprite::draw2D(float x, float y, float angle, float alpha, bool reverseX, bool reverseY) {
	

	x -= camera.x;
	y -= camera.y;
	
	pgeTextureActivate(texture_);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
	sceGuColor(pgeGfxColorSub(color, pgeGfxCreateColor(0,0,0, 255 - (unsigned char) ((alpha > 255) ? 255 : alpha))));
	
	float c, s;
	pgeMathSincos(angle, &s, &c);
	
	float cw = c*(w_*scale.x)/2;
	float sw = s*(w_*scale.x)/2;
	float ch = c*(h_*scale.y)/2;
	float sh = s*(h_*scale.y)/2;
	
	pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory(sizeof(pgeVertTV) * 4);
	
	if (reverseX) {
		vertices[0].u = rect_.x;
		vertices[0].v = rect_.y+ rect_.h;
		vertices[1].u = rect_.x;
		vertices[1].v = rect_.y;
		vertices[2].u = rect_.x + rect_.w;
		vertices[2].v = rect_.y;
		vertices[3].u = rect_.x + rect_.w;
		vertices[3].v = rect_.y + rect_.h;
	} else if (reverseY) {
		vertices[0].u = rect_.x + rect_.w;
		vertices[0].v = rect_.y;
		vertices[1].u = rect_.x + rect_.w;
		vertices[1].v = rect_.y + rect_.h;
		vertices[2].u = rect_.x;
		vertices[2].v = rect_.y + rect_.h;
		vertices[3].u = rect_.x;
		vertices[3].v = rect_.y;
	} else {
		vertices[0].u = rect_.x;
		vertices[0].v = rect_.y;
		vertices[1].u = rect_.x;
		vertices[1].v = rect_.y + rect_.h;
		vertices[2].u = rect_.x + rect_.w;
		vertices[2].v = rect_.y + rect_.h;
		vertices[3].u = rect_.x + rect_.w;
		vertices[3].v = rect_.y;
	}

	vertices[0].x = x - cw + sh;
	vertices[0].y = y - sw - ch;
	vertices[1].x = x - cw - sh;
	vertices[1].y = y - sw + ch;
	vertices[2].x = x + cw - sh;
	vertices[2].y = y + sw + ch;
	vertices[3].x = x + cw + sh;
	vertices[3].y = y + sw - ch;
	
	sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 4, 0, vertices);

	setScale(1,1);
	setColor(0xffffffff);
}
