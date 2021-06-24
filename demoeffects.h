// Header File
// Created 2020/5/30; 22:49:56

//scroller
void initScroller();
void doScroller();

//rotozoomer
void rotozoomerInit();
void doRotozoomer();

//3d
void init3d();
void do3d();

//plasma
void initPlasma();
void doPlasma();

void (*doDemo[4])();

void (*initDemo[4])();