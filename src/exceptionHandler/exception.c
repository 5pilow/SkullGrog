#include <pspkernel.h>
#include <pspsdk.h>
#include "exceptionHandler/exception.h"
#include <pge/pgeGfx.h>
#include <pge/pgeMath.h>
#include <pge/pgeControls.h>
#include <stdio.h>
#include <string.h>
#include "crashed.c" // Texture

PspDebugRegBlock exception_regs;
extern SceModule module_info;
extern int _ftext;

void ExceptionHandler(PspDebugRegBlock * regs) {
	
	pgeTexture* tex = pgeTextureLoadMemory(crashed, size_crashed, PGE_RAM, 0);	
	pgeGfxStartDrawing();
	pgeGfxClearScreen(0xffff0000);
	pgeTextureActivate(tex);
	pgeGfxDrawTextureEasy(tex,40,107,0,255);
	pgeGfxEndDrawing();
	pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	
    while (pgeRunning()) {
		pgeControlsUpdate();
		if (pgeControlsPressed(PGE_CTRL_CROSS)) sceKernelExitGame();
	}
}

void initExceptionHandler()
{
   SceKernelLMOption option;
   int args[2], fd, modid;

   memset(&option, 0, sizeof(option));
   option.size = sizeof(option);
   option.mpidtext = PSP_MEMORY_PARTITION_KERNEL;
   option.mpiddata = PSP_MEMORY_PARTITION_KERNEL;
   option.position = 0;
   option.access = 1;

   if((modid = sceKernelLoadModule("exception.prx", 0, &option)) >= 0) {
      args[0] = (int)ExceptionHandler;
      args[1] = (int)&exception_regs;
      sceKernelStartModule(modid, 8, args, &fd, NULL);
   }
}
