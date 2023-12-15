#include <Windows.h>

#define LIST_SIZE sizeof(LINKED_LIST)
#define NODE_SIZE sizeof(NODE)


typedef struct Node
{
    PVOID pvData;
    PNODE pnNext;

}NODE, *PNODE;

typedef struct LinkedList
{
    DWORD dwLength;
    PNODE pnHead;

}LINKED_LIST, *PLINKED_LIST;


PLINKED_LIST CreateLinkedList();


PNODE CreateNode(PNODE pnNext, PVOID pvData, SIZE_T stSize);


PNODE GetNode(PLINKED_LIST plList, DWORD dwIndex);


BOOL AddEnd(PLINKED_LIST plList, PVOID pvData, SIZE_T stSize);


BOOL AddFront(PLINKED_LIST plList, PVOID pvData, SIZE_T stSize);


void InsertNode(PLINKED_LIST plList, DWORD dwIndex, PVOID pvData, SIZE_T stSize);


void RemoveNode(PLINKED_LIST plList, void (*fptr)(void *));


void RemoveSelected(PLINKED_LIST plList, DWORD dwIndex, void (*fptr)(void *));


void DestroyList(PLINKED_LIST plList, void (*fptr)(void *));


void TraverseList(PLINKED_LIST plList, void (*fptr)(void *));


void SortList(PLINKED_LIST plList, int (*fptr)(void *, void *));


void FindData(PLINKED_LIST plList, int (*fptr)(void *, void *), PVOID pvValue);
