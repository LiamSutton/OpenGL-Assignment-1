#include <stdio.h>
#include "chControl.h"


void controlInit(chControl& rControl) 
{
	rControl.m_abFlags[csg_uiControlDrawGrid] = true;

	rControl.m_abFlagsChanged[csg_uiControlDrawGrid] = true;
}

bool controlActive(chControl& rControl, unsigned uiIndex) 
{
	return rControl.m_abFlags[uiIndex];
}

void controlSet(chControl& rControl, unsigned uiIndex, bool bValue) 
{
	rControl.m_abFlags[uiIndex] = bValue;
	rControl.m_abFlagsChanged[uiIndex] = true;
}

bool controlChanged(chControl& rControl, unsigned uiIndex) 
{
	return rControl.m_abFlagsChanged[uiIndex];
}

void controlToggle(chControl& rControl, unsigned uiIndex)
{
	rControl.m_abFlags[uiIndex] = !rControl.m_abFlags[uiIndex];
	rControl.m_abFlagsChanged[uiIndex] = true;
}

void controlChangeResetAll(chControl& rControl)
{
	rControl.m_abFlagsChanged[csg_uiControlDrawGrid] = false;
}
