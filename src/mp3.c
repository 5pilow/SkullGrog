#include "mp3.h"

// Input and Output buffers
char	mp3Buf[16*1024]  __attribute__((aligned(64)));
short	pcmBuf[16*(1152/2)]  __attribute__((aligned(64)));
int paused = 0;
SceUID mp3ThreadId;

int fd;
int handle;
int channel;
int samplingRate;
int numChannels;
int lastDecoded;
int volume;
int numPlayed;
int loop;
u32 outbits;
char mp3Name[50];
int max_vol = 0x8000;
int volumePercent = 50;


int fillStreamBuffer( int fd, int handle ) {
	
	char* dst;
	int write;
	int pos;
	sceMp3GetInfoToAddStreamData( handle, (SceUChar8**)&dst, &write, &pos);
	sceIoLseek32( fd, pos, SEEK_SET );
	int read = sceIoRead( fd, dst, write );
	if (read == 0) {
		return 0;
	}
	sceMp3NotifyAddStreamData( handle, read );

	return (pos > 0);
}

int sleepMp3() {

	paused = 1;
	sceKernelSuspendThread(mp3ThreadId);
	sceIoClose(fd);
	return 0;
}

int resumeMp3() {
	
	paused = 0;
	fd = sceIoOpen(mp3Name, PSP_O_RDONLY, 0777 );
	sceKernelResumeThread(mp3ThreadId);
	return 0;
}

int playMp3(SceSize args, void *argp) {

	fd = sceIoOpen(mp3Name, PSP_O_RDONLY, 0777 );
	
	sceMp3InitResource();

	SceMp3InitArg mp3Init;
	mp3Init.mp3StreamStart = 0;
	mp3Init.mp3StreamEnd = sceIoLseek32( fd, 0, SEEK_END );
	mp3Init.unk1 = 0;
	mp3Init.unk2 = 0;
	mp3Init.mp3Buf = mp3Buf;
	mp3Init.mp3BufSize = sizeof(mp3Buf);
	mp3Init.pcmBuf = pcmBuf;
	mp3Init.pcmBufSize = sizeof(pcmBuf);
	
	handle = sceMp3ReserveMp3Handle( &mp3Init );
	
	fillStreamBuffer( fd, handle );
	
	sceMp3Init( handle );
	
	channel = -1;
	samplingRate = sceMp3GetSamplingRate( handle );
	numChannels = sceMp3GetMp3ChannelNum( handle );
	lastDecoded = 0;
	volume = max_vol/2;
	numPlayed = 0;
	paused = 0;
	loop = 0;

	while (1) {

		if (sceMp3CheckStreamDataNeeded( handle ) > 0) {
			fillStreamBuffer( fd, handle );
		}

		short* buf;
		int bytesDecoded;
		int retries = 0;
		
		for (;retries<1;retries++)
		{
			bytesDecoded = sceMp3Decode( handle, &buf );
			if (bytesDecoded>0)
				break;
			
			if (sceMp3CheckStreamDataNeeded( handle )<=0)
				break;
			
			if (!fillStreamBuffer( fd, handle )) {
				numPlayed = 0;
			}
		}
		
		if (bytesDecoded==0 || bytesDecoded==0x80671402) {
			paused = 1;
			sceMp3ResetPlayPosition( handle );
			numPlayed = 0;
		} else {
			if (channel<0 || lastDecoded!=bytesDecoded)
			{
				if (channel>=0)
					sceAudioSRCChRelease();
				
				channel = sceAudioSRCChReserve( bytesDecoded/(2*numChannels), samplingRate, numChannels );
			}
			numPlayed += sceAudioSRCOutputBlocking( volume, buf );
		}

		sceKernelDelayThread(1000 * 15);
	}
	
	return 0;
}



int initMP3(char* fileName) {

	sprintf(mp3Name, "%s", fileName);

	
	
	/// Load modules
	sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);
	sceUtilityLoadModule(PSP_MODULE_AV_MP3);
	/// Create Mp3 thread
	mp3ThreadId = sceKernelCreateThread("mp3", playMp3, 11 ,256 * 1024,PSP_THREAD_ATTR_USER, NULL);
	sceKernelStartThread(mp3ThreadId, 0, NULL);
	
	return 0;
}

int setVolume(int newVolume) {

	volumePercent = newVolume;
	volume = (int) (max_vol * ((float)volumePercent/100) );

}

int getVolume() {

	return volumePercent;
}
