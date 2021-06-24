#define NUM_OF_BG_DOTS 72

#include "global.h"
#include "3dmodels.h"

/* Cube
static int DotArray[8][6] = 
{
	{-256, 256, -256, -1, -1, -1},
	{256, 256, -256, 0, -1, -1},
	{256, 256, 256, 1, -1, -1},
	{-256, 256, 256, 2, 0, -1},
	{-256, -256, -256, 0, -1, -1},
	{256, -256, -256, 1, 4, -1},
	{256, -256, 256, 2, 5, -1},
	{-256, -256, 256, 3, 6, 4}
};
*/

static int DotPos[128][2];

static int BackgroundDots[NUM_OF_BG_DOTS][3];

#define MiddleX 80
#define MiddleY 50

struct Camera {
	long OfsX;
	long OfsY;
	long OfsZ;
	unsigned char Pitch;
	unsigned char Roll;
	unsigned char Yaw;
};

static struct Camera CamModel, CamBg;

static struct Model CurModel;

static int ModelNum;

static char SinPitch, CosPitch, SinRoll, CosRoll, SinYaw, CosYaw;

static char KeydownInternal;

static void initbg() {
	randrst();
	int i = 0;
	while (i < NUM_OF_BG_DOTS) {
		int k = 0;
		while (k < 3) {
			int pos = (rand16() - 32768) / 20;
			if (pos > 0 && pos < 280) pos = 320;
			if (pos < 0 && pos > -280) pos = -320;
			BackgroundDots[i][k++] = pos;
		}
		i ++ ;
	}
}

void init3d() {
	GrayOff();
	setspeed(125);
	
	timerFlag = 0;
	ModelNum = step = 0;
	KeydownInternal = 0;

	initbg();

	CurModel = ModelList[ModelNum];

	CamModel.OfsX = CamModel.OfsY = 0;
	CamModel.OfsZ = 65535;
	CamModel.Pitch = CamModel.Roll = CamModel.Yaw = 0;
	
	CamBg.OfsX = CamBg.OfsY = CamBg.OfsZ = 0;
	CamBg.Pitch = CamBg.Roll = CamBg.Yaw = 0;
}

void calcang(struct Camera Cam) {
	SinPitch = sine(Cam.Pitch);
	CosPitch = cosine(Cam.Pitch);
	SinRoll = sine(Cam.Roll);
	CosRoll = cosine(Cam.Roll);
	SinYaw = sine(Cam.Yaw);
	CosYaw = cosine(Cam.Yaw);
}

static int *calc3d(int *DotData, struct Camera Cam) {
	long px = DotData[0];
	long py = DotData[1];
	long pz = DotData[2];
	
	long npx, npy, npz;
	
	npz = pz*CosYaw - px*SinYaw;
	npx = px*CosYaw + pz*SinYaw;
	
	pz = npz>>8;
	px = npx>>8;
	
	npz = pz*CosPitch - ((py*SinPitch)>>1);
	npy = py*CosPitch + ((pz*SinPitch)<<1);
	
	pz = (npz>>8) + (Cam.OfsZ>>4);
	py = (npy>>8);
	
	npx = px*CosRoll - py*SinRoll;
	npy = py*CosRoll + px*SinRoll;
	
	px = (npx>>2) + (Cam.OfsX<<2);
	py = (npy>>2) - (Cam.OfsY<<2);
	
	int zdiv = pz + 216;
	int cx = 255;
	int cy = 255;
	if (zdiv != 0) {
		cx = MiddleX + px/zdiv;
		cy = MiddleY + py/zdiv;
	}
	
	static int finalpos[2];
	finalpos[0] = cx;
	finalpos[1] = cy;
	
	return finalpos;
}

static void NextModel() {
	ModelNum ++ ;
	if (ModelNum == 4) ModelNum = 0;
	CurModel = ModelList[ModelNum];
}

void do3d() {
	memset(lcdbuf, 0, LCD_SIZE);
	
	if (_keytest(RR_UP)) {
		if (!KeydownInternal) {
			CamModel.OfsZ = -480;
			time = 0;
			step = 1;
			NextModel();
		}
		KeydownInternal = 1;
	} else KeydownInternal = 0;
	
	//Model
	switch (step) {
		case 0:
			CamModel.OfsZ -= (CamModel.OfsZ + 2048) >> 2;
			if (CamModel.OfsZ < -480) {
				CamModel.OfsZ = -480;
				time = 0;
				step ++ ;
			}
			break;
		case 1:
			time ++ ;
			if (time >= 100) step ++ ;
			break;
		case 2:
			CamModel.OfsZ = (((CamModel.OfsZ + 768)*6) >> 2) - 768;
			if (CamModel.OfsZ > 65535) {
				CamModel.OfsZ = 65535;
				NextModel();
				step = 0;
			}
			break;
	}
	
	CamModel.Yaw += 4;
	CamModel.Pitch += 3;
	CamModel.Roll += 2;
	calcang(CamModel);
	
	unsigned char i = 0;
	while (i < CurModel.NumOfDots) {
		
		int index = i << 3;
		
		int *result = calc3d(&CurModel.ModelData[index], CamModel);
		int cx = result[0];
		int cy = result[1];
		
		DotPos[i][0] = cx;
		DotPos[i][1] = cy;
		
		unsigned char k = 3;
		while (k < 6) {
			int subindex = index + k;
			if (CurModel.ModelData[subindex] == -1) break;
			unsigned char DotIndex = CurModel.ModelData[subindex];
			LineFromTo(cx, cy, DotPos[DotIndex][0], DotPos[DotIndex][1]);
			k ++ ;
		}
		
		if (k == 3) Dot2(cx, cy);
		
		i ++ ;
	}
	
	//Background
	
	CamBg.Yaw += 2;
	CamBg.Pitch -= 1;
	CamBg.Roll -= 1;
	calcang(CamBg);
	
	i = 0;
	while (i < NUM_OF_BG_DOTS) {
		
		int *result = calc3d(BackgroundDots[i], CamBg);
		int cx = result[0];
		int cy = result[1];
		
		Dot2(cx, cy);
		
		i ++ ;
	}
	
	memcpy(LCD_MEM, lcdbuf, LCD_SIZE);
	
	//printf_xy(0, 0, "%d", time);
}