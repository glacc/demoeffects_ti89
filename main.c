// C Source File
// Created 2019/12/11; 22:37:23

#include "global.h"

INT_HANDLER interrupt1;
INT_HANDLER interrupt5;
unsigned char Original_Rate;
char OrigFont;

unsigned char showCPU;
unsigned int curspd;

DEFINE_INT_HANDLER (int5update) {
	if (timerFlag < 2) timerFlag ++ ;
}

void setspeed(unsigned char spd) {
	while (*(volatile unsigned char*)0x600017 != 0);
	while (*(volatile unsigned char*)0x600017 == 0);
	TMR_ = spd;
	curspd = 255 - spd;
}

void InitEffectList() 
{
	doDemo[0] = &doScroller;
	doDemo[1] = &doRotozoomer;
	doDemo[2] = &do3d;
	doDemo[3] = &doPlasma;
	initDemo[0] = &initScroller;
	initDemo[1] = &rotozoomerInit;
	initDemo[2] = &init3d;
	initDemo[3] = &initPlasma;
}

void demoInit() {
	
	timerFlag = 0;
	
	OrigFont = FontGetSys();
	
	interrupt1 = GetIntVec(AUTO_INT_1);
	interrupt5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5, int5update);
	while (*(volatile unsigned char*)0x600017 != 0);
	while (*(volatile unsigned char*)0x600017 == 0);
	Original_Rate = *(volatile unsigned char*)0x600017;
	setspeed(SPEED_DEFAULT);
	
	InitEffectList();
	
	lcdbuf = malloc(LCD_SIZE);
	lcdbuf2 = malloc(LCD_SIZE);
	if ((!lcdbuf) || (!lcdbuf2)) {
		exit(0);	
	}
	memset(LCD_MEM, 0, LCD_SIZE);
	memset(lcdbuf, 0, LCD_SIZE);
	memset(lcdbuf2, 0, LCD_SIZE);
	
	showCPU = 0;
}

void demoEnd() {
	GrayOff();
	free(lcdbuf);
	free(lcdbuf2);
	
	*(volatile unsigned char*)0x600017 = Original_Rate;
	SetIntVec(AUTO_INT_1, interrupt1);
	SetIntVec(AUTO_INT_5, interrupt5);
	
	GKeyFlush();
	
	FontSetSys(OrigFont);
	
	exit(0);
}

void _main() {
	
	demoInit();
	
	initDemo[0]();

	while (_keytest(RR_ENTER));
	
	DrawStr(4, 8, "Demo Effects on TI-89", A_NORMAL);
	DrawStr(8, 32, "[ENTER] Run", A_NORMAL);
	DrawStr(8, 40, "[ESC]   Exit", A_NORMAL);
	
	FontSetSys(F_4x6);
	/*
	char HWVER_STR[23] = "Hardware version : HWx\0";
	HWVER_STR[21] = 0x31 + HW_VERSION;
	DrawStr(14, 70, HWVER_STR, A_NORMAL);
	*/
	DrawStr(12, 52, "[\x16]     Next", A_NORMAL);
	DrawStr(11, 58, "[\x1B]     CPU Usage", A_NORMAL);
	DrawStr(4, 88, "Glacc", A_NORMAL);
	DrawStr(4, 94, "Originally written in 2020", A_NORMAL);
	
	while (!_keytest(RR_ENTER))
	{
		if (_keytest(RR_ESC)) demoEnd();
	}
	
	unsigned char timerNum = 0;
	unsigned char lastTmr = 0;
	while (!_keytest(RR_ESC)) {
		if (_keytest(RR_RIGHT)) {
			if (!keydown) {
				demopos ++ ;
				if (demopos >= 4) demopos = 0;
				
				initDemo[demopos]();
			}
			keydown = 1;
		} else if (_keytest(RR_SHIFT)) {
			if (!keydown) {
				showCPU = showCPU ? 0 : 1;
				memset(LCD_MEM, 0, LCD_SIZE);
			}
			keydown = 1;
		} else keydown = 0;
		
		if (timerFlag) {
			timerNum = TMR_;
			
			doDemo[demopos]();
			
			if (showCPU) {
				ShowString(0, 6, "CPU", LCD_MEM);
				ShowUByteDec(0, 14, lastTmr, LCD_MEM);	
				ShowUByteDec(0, 22, curspd, LCD_MEM);	
			}
			lastTmr = timerFlag > 1 ? curspd : TMR_ - (255 - curspd);
			
			timerFlag -- ;
		}
	}
	
	demoEnd();
}