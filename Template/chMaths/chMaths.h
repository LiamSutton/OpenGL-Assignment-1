#pragma once
#ifdef _DEBUG
#pragma comment(lib,"chMathsD")
#else
#pragma comment(lib,"chMathsR")
#endif

typedef float chMatrix4[16];
typedef float chVector4[4];

const static float cs_fPi = 3.14159265f;
const static float cs_fHalfPi = 1.57079633f;
static bool s_bMathsInit=false;

void initMaths();
float degToRad(float f);
float redToDeg(float f);
float randFloat(float fMin=0.0f, float fMax=1.0f);
float mathsRadiusOfSphereFromVolume(float fVolume);
float mathsDimensionOfCubeFromVolume(float fVolume);
float mathsRadiusOfConeFromVolume(float fVolume);
