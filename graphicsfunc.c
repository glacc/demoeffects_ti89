// C Source File
// Created 2020/5/28; 20:09:55

#include "global.h"
#include "glaccfx.h"

inline void Dot(int x, int y) {
	if (!(x < 160 && x >= 0 && y < 100 && y >= 0)) return;
	lcdbuf[(x>>3) + y*30] |= 0x80 >> (x&0x07);
}

inline void Dot2(int x, int y) {
	if (!(x < 160 && x >= 0 && y < 100 && y >= 0)) return;
	char shiftX = (x&0x07);
	
	char *ptr = lcdbuf + (x>>3) + y*30;
	char d1 = 0xC0 >> shiftX;
	
	*ptr |= d1;
	*(ptr + 30) |= d1;
	if (shiftX == 7) {
		d1 = 0x80;
		*(ptr + 1) |= d1;
		*(ptr + 31) |= d1;
	}
}

inline void LineFromTo(short x1, short y1, short x2, short y2)
{
  if (!(x1 < 160 && x1 >= 0 && x2 < 160 && x2 >= 0 && y1 < 100 && y1 >= 0 && y2 < 100 && y2 >= 0)) return;
  short x = x1, y = y1;
  short dx = abs (x2 - x1), dy = abs (y2 - y1);
  short ystep = (y1 < y2) ? 1 : -1, pystep = 30 * ystep;
  short mov = dx ? 0 : -1;
  unsigned char *ptr = (char*)lcdbuf + 30 * y + (x >> 3);
  short mask = 1 << (~x & 7);
  if (x1 < x2)
    while (x != x2 || y != y2)
      {
        *ptr |= mask;
        if (mov < 0) y += ystep, ptr += pystep, mov += dx;
        else
          {
            mov -= dy;
            if (++x & 7) mask >>= 1;
            else ptr++, mask = 0x80;
          }
      }
  else
    while (x != x2 || y != y2)
      {
        *ptr |= mask;
        if (mov < 0) y += ystep, ptr += pystep, mov += dx;
        else
          {
            mov -= dy;
            if (x-- & 7) mask <<= 1;
            else ptr--, mask = 1;
          }
      }
}

inline void ShowUByteDec(int x, int y, unsigned char num, char *buffer)
{
	if (x < 0 || x > 168 || y < 0 || y > 168) return;
	char i = 2;
	y += 8;
	int px = x + 24;
	
	while (i >= 0)
	{
		int n = num;
		num /= 10;
		n -= num*10;
		gsprite8(px, y, font8x8[0x30 + n], buffer);
		px -= 8;
		i -- ;
	}
}

inline void ShowString(int x, int y, const char *txt, char *buffer)
{
	if (x < 0 || x > 168 || y < 0 || y > 168) return;
	int xorig = x;
	y += 8;
	while (*txt)
	{
		if (*txt == '\n') {
			y += 8;
			x = xorig;
			txt ++ ;
			continue;	
		}
		gsprite8(x += 8, y, font8x8[(int)*txt++], buffer);
		if (x > 168) return;
	}
}