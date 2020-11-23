#pragma once

#ifdef _DEBUG
#pragma comment(lib,"chUtilitiesD")
#else
#pragma comment(lib,"chUtilitiesR")
#endif

void utilitiesColourToMat(const float* pafCol, float fShininess, bool bEmisive=false);
float* utilitiesScreenProportionToViewport(float fX, float fY, int* aiViewport, float* pF);
