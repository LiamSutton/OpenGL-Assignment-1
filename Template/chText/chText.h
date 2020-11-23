#pragma once
#ifdef _DEBUG
#pragma comment(lib,"chTextD")
#else
#pragma comment(lib,"chTextR")
#endif

void buildFont();
void killFont();
void outlinePrint(char* acString, bool bCentre=true);
 