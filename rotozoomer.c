#include "global.h"

static unsigned char rz_ox, rz_oy;
static unsigned int rz_cang, rz_sang, rz_cx, rz_cy, rz_sx, rz_sy, rz_rang;

void rotoZoomer(unsigned char vecx, unsigned char vecy, long ofx, long ofy, int sclx, int scly) {
	unsigned char fx, fy, px, py, _d, d;
	long lx, ly, ilx, ily;
	long ax, ay;
	long bx, by;
	char *lcdp;
	
	memset(lcdbuf+183, 0, 2640);
	ax = (cosine_(vecx) >> 8)*sclx;
	ay = (sine_(vecy) >> 8)*scly;
	bx = (cosine_(vecx+64) >> 8)*sclx;
	by = (sine_(vecy+64) >> 8)*scly;
	ilx = (ofx<<12) - ax*28 - bx*28;
	ily = (ofy<<12) - ay*22 - by*22;
	py = 0;
	
	lcdp = lcdbuf + 152;
	while (py < 44) {
		px = 0;
		ilx = ilx + bx;
		ily = ily + by;
		lx = ilx;
		ly = ily;
		
		d = 0;
		_d = 0xC0;
		lcdp = lcdbuf + py*60 + 183;
		*(lcdp - 1) = *(lcdp + 29) = 0x01;
		
		while (px < 56) {
			lx = lx + ax;
			ly = ly + ay;
			fx = lx >> 10;
			fy = ly >> 10;
			
			if ((rotozoomer_texture[(fx >> 3) + (fy << 5)] & (0x80 >> (fx & 0x07))))
				*lcdp = *(lcdp + 30) |= _d;
			
			d ++;
			_d = _d >> 2;
			
			if (d == 4) {
				lcdp ++;
				_d = 0xC0;
				d = 0;
			}
			
			px ++ ;
		}
		
		*lcdp = *(lcdp + 30) = 0x80;
		py ++ ;
	}
	
	memcpy(LCD_MEM+153, lcdbuf+153, 2730);
}

void rotozoomerInit() {
	GrayOff();
	
	setspeed(130);
	
	memset(LCD_MEM, 0, LCD_SIZE);
	memset(lcdbuf, 0, LCD_SIZE);
	
	LineFromTo(23,5,136,5);
	LineFromTo(23,94,136,94);
	rz_sx = rz_sy = rz_cx = rz_cy = rz_ox = rz_oy = rz_sang = rz_cang = 0;
	rz_rang = 120;
	
	time = step = 0;
}

void doRotozoomer() {
	rz_cx = sine_(rz_cang)>>8;
	rz_cy = -cosine_(rz_cang)>>8;
	rz_cang += 2;
	
	if (step == 0) {
		time ++ ;
		rz_sx = rz_sy = (sine_(time<<1)>>8) + time;
		if (time == 32) {
			time = 0;
			step ++ ;
		}
	} else if (step == 1) {
		time ++ ;
		if (time == 165 && step == 1) {
			time = 0;
			step ++ ;
		}
	}
	
	if (step >= 1) {
		if (time > 10 || step >= 2) rz_ox = rz_oy -= 3;
		rz_sang += 3;
		if (step != 2) {
			rz_sx = rz_sy = 95 + (cosine_(rz_sang)>>9);
		} else {
			rz_sang += 2 ;
			
			if (rz_sang < 2048) rz_sang = 2048;
			
			if (rz_sx > ((rz_sang-2048)>>4)+1) rz_sx = rz_sy -= ((rz_sang-2048)>>4)+1;
			else {
				step = 3;
				time = 0;
				
				memset(lcdbuf+183, 0, 2640);
				LineFromTo(23,6,23,94);
				LineFromTo(136,6,136,94);
				memcpy(LCD_MEM+153, lcdbuf+153, 2730);
			}
		}
	}
	
	if (step != 3) {
		rotoZoomer(rz_ox, rz_oy, rz_cx, rz_cy, rz_sx, rz_sy);
	} else {
		time ++ ;
		if (time == 4) rotozoomerInit();
	}
	
}