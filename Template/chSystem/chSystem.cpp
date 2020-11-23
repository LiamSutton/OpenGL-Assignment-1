#include "stdafx.h"
#include <math.h>
#include "chSystem.h"
#include <streambuf>

void initSystem(chSystem* pSystem)
{
	if(pSystem)
	{
		initList(&(pSystem->m_llArcs), csg_uiArc);
		initList(&(pSystem->m_llNodes), csg_uiNode);
	}
}

chNode* initNode(chNode* pNode, unsigned int uiId, float* pfPosition, float fMass, const char* acName)
{
	if(pNode)
	{
		vecInitPVec(pNode->m_afPosition);
		vecCopy(pfPosition, pNode->m_afPosition);
		pNode->m_fMass = fMass;
		sprintf_s(pNode->m_acName, "%s", acName);
		pNode->m_uiId = uiId;
		pNode->m_uiContinent = 0;
		pNode->m_uiWorldSystem = 0;
	}

	return pNode;
}

chArc* initArc(chArc* pArc, chNode* pNode0, chNode* pNode1, float fSpringCoef, float fIdealLen)
{
	if(pArc)
	{
		pArc->m_pNode0 = pNode0;
		pArc->m_pNode1 = pNode1;
		pArc->m_fSpringCoef = fSpringCoef;
		pArc->m_fIdealLen = fIdealLen;
	}
	return pArc;
}

void addNode(chSystem* pSystem, chNode* pNode)
{
	if(pSystem && pNode) pushTail(&(pSystem->m_llNodes), initElement(new chLinkedListElement, pNode, csg_uiNode));
}

void visitArcs(chSystem* pSystem, arcFunction* pArcFunction)
{
	if (pSystem && pArcFunction)
	{
		for (chLinkedListElement *pE = pSystem->m_llArcs.m_pHead; pE; pE = pE->m_pNext)
		{
			if (pE->m_uiType == csg_uiArc && pE->m_pData)
			{
				pArcFunction((chArc*)pE->m_pData);
			}
		}
	}
}

void visitNodes(chSystem* pSystem, nodeFunction* pNodeFunction)
{
	if(pSystem && pNodeFunction)
	{
		for(chLinkedListElement *pE=pSystem->m_llNodes.m_pHead;pE;pE=pE->m_pNext)
		{
			if(pE->m_uiType==csg_uiNode && pE->m_pData)
			{
				pNodeFunction((chNode*)pE->m_pData);
			}
		}
	}
}

chNode* nodeById(chSystem *pSystem, unsigned int uiId)
{
	if(pSystem && uiId)
		for(chLinkedListElement *pE=pSystem->m_llNodes.m_pHead;pE;pE=pE->m_pNext)
			if (pE->m_uiType == csg_uiNode && pE->m_pData && ((chNode*)pE->m_pData)->m_uiId == uiId) return (chNode*)pE->m_pData;
	return 0;
}

void addArc(chSystem* pSystem, chArc* pArc)
{
	if (pSystem && pArc) pushTail(&(pSystem->m_llArcs), initElement(new chLinkedListElement, pArc, csg_uiArc));
}
