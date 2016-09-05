#ifndef SKULLGROG_MP3_H
#define SKULLGROG_MP3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pspkernel.h>
#include <psppower.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdio.h>
#include <pspaudio.h>
#include <pspmp3.h>
#include <psputility.h>


int fillStreamBuffer( int fd, int handle );

int sleepMp3();

int resumeMp3();

int playMp3(SceSize args, void *argp);

int CallbackThread(SceSize args, void *argp);

int initMP3(char* fileName);

int setVolume(int newVolume);

int getVolume();

#ifdef __cplusplus
}
#endif

#endif
