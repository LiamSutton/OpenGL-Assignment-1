#pragma once
#ifdef _DEBUG
#pragma comment(lib,"chCameraD")
#else
#pragma comment(lib,"chCameraR")
#endif

enum tribool
{
	tri_null,
	tri_pos,
	tri_neg
};

typedef struct _chCameraInput
{
	bool m_bExplore;
	bool m_bFly;
	bool m_bMouse;
	bool m_bMousePan;
	int m_aiMouseStart[2];
	int m_aiMouseLast[2];
	tribool m_tbKeyTravel;
	tribool m_tbKeyPanHori;
	tribool m_tbKeyPanVert;
	float m_fAngularSensitivity;
	float m_fLinearSensitivity;
} chCameraInput;


typedef struct _chCamera
{
	float m_fVP[4];
	float m_fVT[4];
	float m_fVR[4];
	float m_fVU[4];
	float m_fVD[4];
	float m_fNear;
	float m_fFar;
	float m_afMat[16];
	float m_afInvMat[16];
	float m_afRotMat[16];
	float m_afRotInvMat[16];
	bool m_bChanged;
	bool m_bInvChanged;
	bool m_bRotChanged;
	bool m_bRotInvChanged;
	float m_afProjMat[16];
	int m_aiViewport[4];
	bool m_bViewportChanged;
} chCamera;

// input mode functions
void camInputInit(chCameraInput &rInput);
void camInputExplore(chCameraInput &rInput, bool bState);
void camInputFly(chCameraInput &rInput, bool bState);
void camInputMouse(chCameraInput &rInput, bool bState);
void camInputMousePan(chCameraInput &rInput, bool bState);
void camInputTravel(chCameraInput &rInput, tribool tbState);
void camInputPanHori(chCameraInput &rInput, tribool tbState);
void camInputPanVert(chCameraInput &rInput, tribool tbState);
void camInputSetMouseStart(chCameraInput &rInput, int iX, int iY);
void camInputSetMouseLast(chCameraInput &rInput, int iX, int iY);

void camProcessInput(chCameraInput &rInput, chCamera &rCam);
int* camViewport(chCamera &rCam);
void camSetViewport(chCamera &rCam, int iX, int iY, int iWidth, int iHeight);
bool camViewportChanged(chCamera &rCam);
void camResetViewportChanged(chCamera &rCam);

// basic functions
void camInit(chCamera &rCam);
void camReset(chCamera &rCam);
void camCopy(chCamera &rIn, chCamera &rOut);
void camApply(chCamera &rCam);
float* camObjMat(chCamera &rCam);
float* camObjMatInv(chCamera &rCam);
float* camRotMat(chCamera &rCam);
float* camRotMatInv(chCamera &rCam);

// explore mode functions
void camExploreRotate(chCamera &rCam, float fUp, float fRight);
void camExploreTravel(chCamera &rCam, float fDisp);
void camExplorePan(chCamera &rCam, float fHori, float fVert);
void camExploreUpdateTarget(chCamera &rCam, float *pfTarget);
void camExploreUpdateTargetAndDistance(chCamera &rCam, float fDist, float *pfTarget);

// fly mode functions
void camFlyRotate(chCamera &rCam, float fUp, float fRight);
void camFlyTravel(chCamera &rCam, float fForward, float fHori, float fVert);

// slerp
void camSLERP(chCamera &rCamStart, chCamera &rCamEnd, float fSlerp, chCamera &rCam);

// internal use
void camResolveUp(chCamera &rCam);
void camPrint(chCamera &rCam);

bool renderProject(float fX, float fY, float fZ, float* pfMV, float* pfPV, int* piViewport, float* pfOutput);
bool renderUnProject(float fX, float fY, float fZ, float* pfMV, float* pfPV, int* piViewport, float* pfOutput);

void MultiplyMatrices4by4OpenGL_FLOAT(float* result, float* matrix1, float* matrix2);

void MultiplyMatrixByVector4by4OpenGL_FLOAT(float* resultvector, const float* matrix, const float* pvector);


//This code comes directly from GLU except that it is for float
bool glhInvertMatrixf2(float* m, float* out);
