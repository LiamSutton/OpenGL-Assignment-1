#pragma once

#ifdef _DEBUG
#pragma comment(lib,"chLinkedListD")
#else
#pragma comment(lib,"chLinkedListR")
#endif
typedef struct _chLinkedListElement
{
	_chLinkedListElement *m_pNext;
	_chLinkedListElement *m_pLast;
	void *m_pData;
	unsigned int m_uiType;
} chLinkedListElement;

typedef struct _chLinkedList
{
	chLinkedListElement *m_pHead;
	chLinkedListElement *m_pTail;
	unsigned int m_uiType;
} chLinkedList;

typedef void (chListDeletor)(chLinkedListElement *pElement);
typedef void (chListActor)(chLinkedListElement *pElement);

void initList(chLinkedList *pList, unsigned int uiType=0);
void destroyList(chLinkedList *pList, chListDeletor *pDeletor=0);

chLinkedListElement* initElement(chLinkedListElement *pElement, void* pData = 0, unsigned int uiType = 0);
chLinkedListElement* destroyElement(chLinkedListElement *pElement, chListDeletor *pDeletor=0);

void pushHead(chLinkedList *pList, chLinkedListElement *pElement);
void pushTail(chLinkedList *pList, chLinkedListElement *pElement);
chLinkedListElement* head(chLinkedList *pList);
chLinkedListElement* tail(chLinkedList *pList);
unsigned int count(chLinkedList *pList);
chLinkedListElement* item(chLinkedList *pList, unsigned int uiIndex);
void visit(chLinkedList *pList, chListActor *pActor = 0);
chLinkedListElement* popHead(chLinkedList *pList);
chLinkedListElement* popTail(chLinkedList *pList);
bool insertBefore(chLinkedList *pList, chLinkedListElement *pCurrentElement, chLinkedListElement *pNewElement);
bool insertAfter(chLinkedList *pList, chLinkedListElement *pCurrentElement, chLinkedListElement *pNewElement);
bool remove(chLinkedList *pList, chLinkedListElement *pElement);
bool isMember(chLinkedList *pList, chLinkedListElement *pElement);

