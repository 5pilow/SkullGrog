#include "utils.hpp"

#include <stdarg.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <string.h>

Sprite* Utils::boxCorner;
Sprite* Utils::boxEdge1;
Sprite* Utils::boxEdge2;

float Utils::normalize(float n) {
	
	if (n == 0) return 0;
	return n/pgeMathAbs(n);
}

char* Utils::strTime(int time) {
	
	int h = (int)(time/3600);
	time -= h*3600;
	int m = (int)(time/60);
	time -= m*60;
	int s = time;

	std::ostringstream strTime;
	if (h > 0) strTime << h << ":";
	if (m < 10) strTime << "0";
	strTime << m << ":";
	if (s < 10) strTime << "0";
	strTime << s;
	
	return (char*)strTime.str().c_str();
}

char* Utils::virgulesMilliers(int n) {
	
	std::ostringstream oss;
	oss << n;
	return virgulesMilliers((char*)oss.str().c_str());
}

char* Utils::virgulesMilliers(char* n) {
	
	std::string str(n);
	int i = str.size() - 4;
	while (i >= 0) {
		str.insert(i+1,".");
		i -= 3;
	}
	return (char*)str.c_str();
}

void Utils::drawBox(int x, int y, int w, int h) {

	pgeRect rect = {x-w/2, y-h/2, w,h};
	pgeGfxDrawRect(&rect, 0x88000000,0);

	boxCorner->draw2DNoCamera(x-(int)(w/2), y-(int)(h/2), 0, 255, false,false);
	boxCorner->draw2DNoCamera(x-(int)(w/2), y+(int)(h/2), 0, 255, true,false);
	boxCorner->draw2DNoCamera(x+(int)(w/2), y+(int)(h/2), GU_PI, 255, false,false);
	boxCorner->draw2DNoCamera(x+(int)(w/2), y-(int)(h/2), 0, 255, false,true);

	Sprite::setScale(1,h-19);
	boxEdge1->draw2DNoCamera(x-(int)(w/2),y,0,255,false,false);
	Sprite::setScale(1,h-18);
	boxEdge1->draw2DNoCamera(x+(int)(w/2)+1,y-1,GU_PI,255,false,false);

	Sprite::setScale(w-19,1);
	boxEdge2->draw2DNoCamera(x,y-(int)(h/2),0,255,false,false);
	Sprite::setScale(w-18,1);
	boxEdge2->draw2DNoCamera(x-1,y+(int)(h/2)+1,GU_PI,255,false,false);
}

char* Utils::osk(char* text, char* text2) {

	pgeUtilsOskInit(text, text2);
	
	char osktext[512];

	while (true) {
		pgeControlsUpdate();
		pgeGfxStartDrawing();
		pgeGfxClearScreen(0);
		pgeGfxEndDrawing();
		if (pgeUtilsOskUpdate(osktext) > -1) return osktext;
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	}
}

char* Utils::getPSPPseudo() {

	char nickname[128];
	pgeSystemGetNickname(nickname);
	return nickname;
}

void Utils::debug(char* msg, ...) {

	char buffer[256];
	va_list ap;
	va_start(ap, msg);
	vsnprintf(buffer, 256, msg, ap);
	va_end(ap);
	buffer[255] = 0;

	int fd = sceIoOpen("debug.txt", PSP_O_WRONLY|PSP_O_CREAT|PSP_O_APPEND, 0777);
	sceIoWrite(fd, buffer, strlen(buffer));
	sceIoClose(fd);
}

