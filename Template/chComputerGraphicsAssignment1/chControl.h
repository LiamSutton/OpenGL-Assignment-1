#pragma once
#include "chConstants.h"

typedef struct _chControl
{
	bool m_abFlags[csg_uiControlNumFlags];
	bool m_abFlagsChanged[csg_uiControlNumFlags];
	char m_acLayout[csg_uiDefaultStringLength];
} chControl;

void controlInit(chControl& rControl);
bool controlActive(chControl& rControl, unsigned int uiIndex);
void controlSet(chControl& rControl, unsigned int uiIndex, bool bValue);
bool controlChanged(chControl& rControl, unsigned int uiIndex);
void controlChangeResetAll(chControl& rControl);
void controlToggle(chControl &rControl, unsigned int uiIndex);
