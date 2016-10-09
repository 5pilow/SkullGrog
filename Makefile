TARGET = Skullgrog

OBJS += R/sprites.c R/map1.c R/map2.c R/map3.c R/map4.c R/map5.c R/map6.c R/map7.c R/map8.c R/map9.c R/dh1.c R/dh2.c R/dh3.c R/logo.c R/doubleGun.c R/douille.c R/explosion.c R/lanceGrenades.c R/laser.c R/mitrailleuse.c R/shotgun.c R/zombie1.c R/zombie2.c R/zombie3.c R/zombie4.c R/logoGorglucks.c R/logoScenery.o R/heartBreath.c R/flamme.c R/gaz.c
OBJS += main.o adhoc.o exceptionHandler/exception.o perso.o human.o zombie.o arme.o sprite.o particles.o game.o system/adhoc.o map.o menu.o splash.o network.o stats.o intraFont/libccc.o intraFont/intraFont.o findpath.o utils.o mp3.o
OBJS += Armes/mitrailleuse.o Armes/lanceFlammes.o Armes/doubleGun.o Armes/laser.o Armes/pacificateur.o Armes/pistolet.o Armes/magnum.o Armes/lanceGrenades.o Armes/shotgun.o Armes/gazeur.o 

#~ BUILD_PRX = 1

CFLAGS = -O3 -frename-registers -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti

LIBS = -lstdc++ 

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Skullgrog
PSP_EBOOT_ICON = ICON0_SkullGrog.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/pge_build.mak
include $(PSPSDK)/lib/build.mak

install : 
	cp -f EBOOT.PBP /media/26EB-46D7/Skullgrog/
firness :
	cp -f EBOOT.PBP /media/0111-6B24/Skullgrog/
	
open :
	geany *.*pp
	geany Armes/*.*pp
	
cleanRessources :
	rm -f sprites.c
	
## texture
R/sprites.c : texture/sprites.png
	bin2c texture/sprites.png build/texture/sprites.c R_sprites

R/map1.c : texture/Map/map1.png
	bin2c texture/Map/map1.png R/map1.c R_map1
R/map2.c : texture/Map/map2.png
	bin2c texture/Map/map2.png R/map2.c R_map2
R/map3.c : texture/Map/map3.png
	bin2c texture/Map/map3.png R/map3.c R_map3
R/map4.c : texture/Map/map4.png
	bin2c texture/Map/map4.png R/map4.c R_map4
R/map5.c : texture/Map/map5.png
	bin2c texture/Map/map5.png R/map5.c R_map5
R/map6.c : texture/Map/map6.png
	bin2c texture/Map/map6.png R/map6.c R_map6
R/map7.c : texture/Map/map7.png
	bin2c texture/Map/map7.png R/map7.c R_map7
R/map8.c : texture/Map/map8.png
	bin2c texture/Map/map8.png R/map8.c R_map8
R/map9.c : texture/Map/map9.png
	bin2c texture/Map/map9.png R/map9.c R_map9
	
R/dh1.c : texture/dh1.png
	bin2c texture/dh1.png R/dh1.c R_dh1
R/dh2.c : texture/dh2.png
	bin2c texture/dh2.png R/dh2.c R_dh2
R/dh3.c : texture/dh3.png
	bin2c texture/dh3.png R/dh3.c R_dh3
	
R/logo.c : texture/logoSkullGrog2.png
	bin2c texture/logoSkullGrog2.png R/logo.c R_logo
R/logoGorglucks.c : texture/logoGorglucks.png
	bin2c texture/logoGorglucks.png R/logoGorglucks.c R_logoGorglucks
R/logoScenery.c : texture/SB2010PSP.png
	bin2c texture/SB2010PSP.png R/logoScenery.c R_logoScenery
	
## Sons
R/doubleGun.c : Sons/doubleGun.wav
	bin2c Sons/doubleGun.wav R/doubleGun.c R_doubleGun
R/douille.c : Sons/douille.wav
	bin2c Sons/douille.wav R/douille.c R_douille
R/explosion.c : Sons/explosion.wav
	bin2c Sons/explosion.wav R/explosion.c R_explosion
R/lanceGrenades.c : Sons/lanceGrenades.wav
	bin2c Sons/lanceGrenades.wav R/lanceGrenades.c R_lanceGrenades
R/laser.c : Sons/laser.wav
	bin2c Sons/laser.wav R/laser.c R_laser
R/mitrailleuse.c : Sons/mitrailleuse.wav
	bin2c Sons/mitrailleuse.wav R/mitrailleuse.c R_mitrailleuse
R/shotgun.c : Sons/shotgun.wav
	bin2c Sons/shotgun.wav R/shotgun.c R_shotgun
R/zombie1.c : Sons/zombie1.wav
	bin2c Sons/zombie1.wav R/zombie1.c R_zombie1
R/zombie2.c : Sons/zombie2.wav
	bin2c Sons/zombie2.wav R/zombie2.c R_zombie2
R/zombie3.c : Sons/zombie3.wav
	bin2c Sons/zombie3.wav R/zombie3.c R_zombie3
R/zombie4.c : Sons/zombie4.wav
	bin2c Sons/zombie4.wav R/zombie4.c R_zombie4
R/heartBreath.c : Sons/heartBreath.wav
	bin2c Sons/heartBreath.wav R/heartBreath.c R_heartBreath
R/flamme.c : Sons/flamme.wav
	bin2c Sons/flamme.wav R/flamme.c R_flamme
R/gaz.c : Sons/gaz.wav
	bin2c Sons/gaz.wav R/gaz.c R_gaz
