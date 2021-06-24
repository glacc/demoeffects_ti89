// C Source File
// Created 2020/5/30; 18:13:42

#include "global.h"

static int PosX, PosY;
static unsigned char PaletteShift;
static char GrayStat;

static char PalettePlasma[8] = {0,1,2,3,3,2,1,0};

void initPlasma() {
	GrayOff();
	
	setspeed(70);
	GrayStat = GrayOn();
	PaletteShift = PosX = PosY = 0;
}

void doPlasma() {
	if (!GrayStat) return;
	
	memset(lcdbuf, 0, LCD_SIZE);
	memset(lcdbuf2, 0, LCD_SIZE);
	
	char *LightPlane = GrayGetPlane(0);
	char *DarkPlane = GrayGetPlane(1);
	
	PaletteShift += 10;
	PosX += 5;
	PosY += 3;
	
	int ofs = 0;
	int ofs1 = 30;
	
	int y = 0;
	while (y < 50) {
		ofs = y * 60;
		ofs1 = ofs + 30;
		
		unsigned char cx = PosX;
		unsigned char cy = PosY + (y << 2);
		
		unsigned char d = 0;
		unsigned char _d = 0xC0;
		
		int Ysum = sinetable256[cy] + PaletteShift;
		
		int x = 0;
		while (x < 80) {
			
			unsigned int sinesum = (sinetable256[cx] + Ysum) >> 5;
			char colornum = PalettePlasma[sinesum&0x07];
			
			if (colornum&0x01) *(lcdbuf + ofs) = *(lcdbuf + ofs1) |= _d;
			if (colornum&0x02) *(lcdbuf2 + ofs) = *(lcdbuf2 + ofs1) |= _d;
			
			d ++;
			_d = _d >> 2;
			
			if (d == 4) {
				ofs ++;
				ofs1 = ofs + 30;
				_d = 0xC0;
				d = 0;
			}
			
			x ++ ;
			cx += 4;
		}
		y ++ ;
	}
	
	memcpy(LightPlane, lcdbuf, LCD_SIZE);
	memcpy(DarkPlane, lcdbuf2, LCD_SIZE);
}