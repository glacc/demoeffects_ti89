// Header File
// Created 2019/12/11; 22:43:30

#include <tigcclib.h>
#include "fastmath.h"
#include "graphicsfunc.h"
#include "textures.h"
#include "demoeffects.h"

#define TMR_ *(volatile unsigned char*)0x600017

#define SPEED_DEFAULT 150

//#define DEBUG

unsigned int time;
unsigned char step;
unsigned char demopos;
unsigned char timerFlag;
char *lcdbuf;
char *lcdbuf2;

unsigned char keydown;

void setspeed(unsigned char spd);

#ifdef DEBUG

#endif