// C Source File
// Created 2019/12/11; 22:41:01

#include "global.h"

int sine_(int x) {
	return sinetable[(x&0xFF)<<1] - 32765;
}

int cosine_(int x) {
	return sinetable[((x+64)&0xFF)<<1] - 32765;
}

char sine(unsigned char x) {
	return sinetable256[x&0xFF] - 128;
}

char cosine(unsigned char x) {
	return sinetable256[(x+64)&0xFF] - 128;
}

static long randseed = 1523563312;

unsigned int rand16() {
	randseed = randseed * 6534675;
	return (unsigned int)randseed;
}

void randrst() {
	randseed = 1523563312;
}