#define BG_STAR_COUNT 36

#include "global.h"
#include "glaccfx.h"

struct Character {
	int PosX;
	char PosY;
	unsigned char Timer;
	unsigned char *SprAddr;
};

struct BgStar {
	int PosX;
	int PosY;
	int MaxTimer;
	int Timer;
};

struct Character CharList[21];
struct BgStar Stars[BG_STAR_COUNT];

static char ScrollTxt[] = "Glacc's text scroller for TI-89 calculator.                    ";

static int TxtPos = 0;

static char time2 = 0;

static char GrayStat;

void initScroller()
{
	GrayOff();
	randrst();

	setspeed(140);
	GrayStat = GrayOn();

	TxtPos = time2 = time = step = 0;
	
	int i = 0;
	while (i < 21)
	{
		CharList[i].PosX = 0;
		CharList[i].PosY = 0;
		CharList[i].Timer = 0;
		i ++ ;
	}
	
	i = 0;
	while (i < BG_STAR_COUNT)
	{
		BgInitLoop:
		Stars[i].PosX = (rand16() & 0x7FFF) >> 7;
		Stars[i].PosY = (rand16() & 0x7FFF) >> 7;
		Stars[i].MaxTimer = (rand16() & 0x7FFF) >> 7;
		Stars[i].Timer = 0;
		while (Stars[i].PosY < 0 || Stars[i].PosY > 100) goto BgInitLoop;
		i ++ ;
	}
}

void doScroller()
{
	if (!GrayStat) return;

	memset(lcdbuf, 0, LCD_SIZE);
	memset(lcdbuf2, 0, LCD_SIZE);
	
	char *LightPlane = GrayGetPlane(0);
	char *DarkPlane = GrayGetPlane(1);
	
	int i;
	if (time2 >= 8)
	{
		i = 0;
		if (ScrollTxt[TxtPos] != 0)
		{
			while (i < 21)
			{
				if (CharList[i].PosX <= 0)
				{
					CharList[i].PosX = 168 - time2 + 8;
					CharList[i].PosY = 58 + (sine(time) >> 3);
					CharList[i].Timer = 0;
					CharList[i].SprAddr = font8x8[(int)ScrollTxt[TxtPos]];
					break;
				}
				i ++ ;
			}
			TxtPos ++ ;
		}
		else TxtPos = 0;
		time2 -= 8;
	}
	
	i = 0;
	while (i < 21)
	{
		if (CharList[i].PosX > 0)
		{
			gsprite8(CharList[i].PosX, CharList[i].PosY + (sine(CharList[i].Timer) >> 4), CharList[i].SprAddr, lcdbuf);
			gsprite8(CharList[i].PosX, CharList[i].PosY + (sine(CharList[i].Timer) >> 4), CharList[i].SprAddr, lcdbuf2);
			CharList[i].PosX -= 3;
			CharList[i].Timer += 4;
		}
		i ++ ;	
	}
	
	i = 0;
	while (i < BG_STAR_COUNT)
	{
		Dot2(Stars[i].PosX, Stars[i].PosY);
		Stars[i].Timer += 120;
		int j = 0;
		while (Stars[i].Timer > Stars[i].MaxTimer)
		{
			Stars[i].PosX -- ;
			Stars[i].Timer -= Stars[i].MaxTimer;
			j ++ ;
			if (j > 3) break;
		}
		if (Stars[i].PosX < 0 || Stars[i].PosY < 0 || Stars[i].PosY > 100)
		{
			Stars[i].PosY = (rand16() & 0x7FFF) >> 7;
			Stars[i].PosX = 160;
			Stars[i].MaxTimer = (rand16() & 0x7FFF) >> 7;
		}
		i ++ ;
	}
	
	time += 5;
	time2 += 3;
	
	memcpy(LightPlane, lcdbuf2, LCD_SIZE);
	memcpy(DarkPlane, lcdbuf, LCD_SIZE);
}