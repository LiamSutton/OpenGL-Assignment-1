#pragma once
#ifdef _DEBUG
#pragma comment(lib,"chSystemD")
#else
#pragma comment(lib,"chSystemR")
#endif

#include <chLinkedList/chLinkedList.h>
#include <chMaths/chVector.h>

typedef struct _chSystem
{
	chLinkedList m_llNodes;
	chLinkedList m_llArcs;
} chSystem;

typedef struct _chNode
{
	unsigned int m_uiId;
	float m_afPosition[4];
	float m_fMass;
	unsigned int m_uiContinent;
	unsigned int m_uiWorldSystem;
	char m_acName[64];
	float m_afForce[3];
	float m_afAcceleration[3];
	float m_afVelocity[3];
} chNode;

typedef struct _chArc
{
	chNode *m_pNode0;
	chNode *m_pNode1;
	float m_fSpringCoef;
	float m_fIdealLen;
} chArc;

const static unsigned int csg_uiNode = 1;
const static unsigned int csg_uiArc = 2;

typedef void (nodeFunction)(chNode *pNode);
typedef void (arcFunction)(chArc *pArc);

void initSystem(chSystem *pSystem);
chNode* initNode(chNode *pNode, unsigned int uiId, float *pfPosition, float fMass, const char *acName);
chArc* initArc(chArc *pArc, chNode *pNode0, chNode *pNode1, float fSpringCoef, float fIdealLen);

void addNode(chSystem *pSystem, chNode *pNode);
void addArc(chSystem *pSystem, chArc *pArc);

chNode* nodeById(chSystem *pSystem, unsigned int uiId);

void visitNodes(chSystem *pSystem, nodeFunction* pNodeFunction);
void visitArcs(chSystem *pSystem, arcFunction* pArcFunction);


