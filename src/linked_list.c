#include "../include/linked_list.h"
#include "../include/debug.h"
#include <Windows.h>
#include <errno.h>
#include <stdio.h>

PLINKED_LIST CreateLinkedList()
{
  PLINKED_LIST plList = NULL;

  plList = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, LIST_SIZE);
  if (NULL == plList)
  {
    ERRORPRINT("List was not allocated properly. Error: %s\n", strerror(errno));
    return plList;
  }
  plList->dwLength = 0;
  plList->pnHead   = NULL;

  return plList;
}

PNODE CreateNode(PNODE pnNext, PVOID pvData, SIZE_T stSize)
{
  PNODE pnNode = NULL;

  pnNode = (PNODE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, NODE_SIZE);
  if (NULL == pnNode)
  {
    ERRORPRINT("Failed to allocate memory for node\n");
    return NULL;
  }

  pnNode->pvData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, stSize);
  if (NULL == pnNode->pvData)
  {
    ERRORPRINT("Failed to allocate memory for node data\n");
    HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pnNode);
    pnNode = NULL;
    return NULL;
  }

  CopyMemory(pnNode->pvData, pvData, stSize);

  if (pnNext != NULL)
  {
    pnNode->pnNext = pnNext;
  }
  else
  {
    pnNode->pnNext = NULL;
  }

  return pnNode;
}

PNODE GetNode(PLINKED_LIST plList, DWORD dwIndex)
{
  PNODE pnCurrent = NULL;

  if (0 > dwIndex || dwIndex >= plList->dwLength || NULL == plList)
  {
    ERRORPRINT("Invalid arguments passed to GetNode()\n");
    return NULL;
  }

  pnCurrent = plList->pnHead;

  for (DWORD i = 1; i <= dwIndex && pnCurrent != NULL; i++)
  {
    pnCurrent = pnCurrent->pnNext;
  }

  return pnCurrent;
}

BOOL AddEnd(PLINKED_LIST plList, PVOID pvData, SIZE_T stSize)
{
  BOOL  bRetVal = FALSE;
  PNODE pnNode  = NULL;
  PNODE pnPrev  = NULL;

  if (NULL == plList || NULL == pvData)
  {
    ERRORPRINT("Invalid arguments passed to AddEnd()\n");
    return bRetVal;
  }

  pnNode = CreateNode(plList->pnHead, pvData, stSize);
  if (NULL == pnNode)
  {
    ERRORPRINT("Failed to create node when adding to end of linked list\n");
    return bRetVal;
  }

  if (plList->dwLength)
  {
    pnPrev = GetNode(plList, plList->dwLength - 1);
    if (NULL == pnPrev)
    {
      ERRORPRINT("Failed to get node\n");
      return bRetVal;
    }
    pnPrev->pnNext = pnNode;
  }
  else
  {
    plList->pnHead = pnNode;
    pnNode->pnNext = NULL;
  }

  plList->dwLength++;
  bRetVal = TRUE;

  return bRetVal;
}

BOOL AddFront(PLINKED_LIST plList, PVOID pvData, SIZE_T stSize)
{
  BOOL  bRetVal = FALSE;
  PNODE pnNode  = NULL;

  if (NULL == plList || NULL == pvData)
  {
    ERRORPRINT("Invalid arguments passed to AddFront()\n");
    return bRetVal;
  }

  pnNode = CreateNode(plList->pnHead, pvData, stSize);
  if (NULL == pnNode)
  {
    ERRORPRINT("Failed to create node when adding to front of linked list\n");
    return bRetVal;
  }

  plList->pnHead = pnNode;
  plList->dwLength++;

  bRetVal = TRUE;
  return bRetVal;
}

BOOL InsertNode(PLINKED_LIST plList, DWORD dwIndex, PVOID pvData, SIZE_T stSize)
{
  BOOL  bRetVal = FALSE;
  PNODE pnNode  = NULL;
  PNODE pnPrev  = NULL;

  if (NULL == plList || NULL == pvData)
  {
    ERRORPRINT("Invalid arguments passed to InsertNode()\n");
    return bRetVal;
  }

  if (0 > dwIndex || dwIndex > plList->dwLength)
  {
    ERRORPRINT("Invalid location in the linked list\n");
    return bRetVal;
  }

  pnNode = CreateNode(NULL, pvData, stSize);
  if (NULL == pnNode)
  {
    ERRORPRINT("Failed to create node when inserting into linked list\n");
    return bRetVal;
  }

  if (0 == dwIndex)
  {
    // Inserting at the front of the list
    pnNode->pnNext = plList->pnHead;
    plList->pnHead = pnNode;
  }
  else
  {
    pnPrev = GetNode(plList, dwIndex - 1);
    if (NULL == pnPrev)
    {
      ERRORPRINT("Failed to get node\n");
      return bRetVal;
    }
    pnNode->pnNext = pnPrev->pnNext;
    pnPrev->pnNext = pnNode;
  }

  plList->dwLength++;
  bRetVal = TRUE;

  return bRetVal;
}

BOOL RemoveNode(PLINKED_LIST plList, VOID (*fptr)(PVOID))
{
  BOOL   bRetVal = FALSE;
  PNODE *ppnHead = NULL;
  PNODE  pnNext  = NULL;

  if (NULL == plList || !plList->dwLength)
  {
    ERRORPRINT("List is either empty or null\n");
    return bRetVal;
  }

  ppnHead = &plList->pnHead;
  pnNext  = (*ppnHead)->pnNext;

  if (NULL != (*ppnHead)->pvData)
  {
    (*fptr)((*ppnHead)->pvData);
  }

  HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, *ppnHead);
  (*ppnHead) = pnNext;

  plList->dwLength--;

  bRetVal = TRUE;
  return bRetVal;
}

BOOL RemoveSelected(PLINKED_LIST plList, DWORD dwIndex, VOID (*fptr)(PVOID))
{
  BOOL  bRetVal   = FALSE;
  PNODE pnCurrent = NULL;
  PNODE pnPrev    = NULL;

  // Check if a valid index was passed
  if (dwIndex >= plList->dwLength)
  {
    ERRORPRINT("Invalid location in the linked list\n");
    return bRetVal;
  }

  if (0 == plList->dwLength)
  {
    ERRORPRINT("Linked list currently does not have any nodes\n");
    return bRetVal;
  }

  if (0 == dwIndex)
  {
    // Removing the head of the list
    pnCurrent      = plList->pnHead;
    plList->pnHead = plList->pnHead->pnNext;
  }
  else if (dwIndex == plList->dwLength - 1)
  {
    // Removing the last node in the list
    pnPrev         = GetNode(plList, dwIndex - 1);
    pnCurrent      = pnPrev->pnNext;
    pnPrev->pnNext = NULL;
  }
  else
  {
    // Removing a node at a specific position in the list
    pnPrev         = GetNode(plList, dwIndex - 1);
    pnCurrent      = pnPrev->pnNext;
    pnPrev->pnNext = pnCurrent->pnNext;
  }

  if (fptr)
  {
    (*fptr)(pnCurrent->pvData);
  }

  HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pnCurrent);
  plList->dwLength--;

  bRetVal = TRUE;
  return bRetVal;
}

BOOL DestroyList(PLINKED_LIST plList, VOID (*fptr)(PVOID))
{
  BOOL bRetVal = FALSE;

  if (NULL == plList)
  {
    ERRORPRINT("Linked list is null\n");
    return bRetVal;
  }

  while (plList->dwLength > 0)
  {
    bRetVal = RemoveNode(plList, fptr);
    if (!bRetVal)
    {
      ERRORPRINT("Failed to remove node\n");
    }
  }

  if (!HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, plList))
  {
    ERRORPRINT("Failed to free memory\n");
    return bRetVal;
  }
  plList = NULL;

  bRetVal = TRUE;
  return bRetVal;
}

BOOL TraverseList(PLINKED_LIST plList, VOID (*fptr)(PVOID))
{
  BOOL  bRetVal   = FALSE;
  PNODE pnCurrent = NULL;

  if (NULL == plList || NULL == fptr)
  {
    ERRORPRINT("Invalid arguments passed to TraverseList()\n");
    return bRetVal;
  }

  pnCurrent = plList->pnHead;

  while (NULL != pnCurrent)
  {
    (*fptr)(pnCurrent->pvData);
    pnCurrent = pnCurrent->pnNext;
  }

  bRetVal = TRUE;
  return bRetVal;
}

BOOL SortList(PLINKED_LIST plList, INT (*fptr)(PVOID, PVOID))
{
  BOOL  bRetVal = FALSE;
  PNODE pnStart = NULL;
  PNODE pnTmp   = NULL;
  PVOID pvTmp   = NULL;

  if (NULL == plList || NULL == fptr)
  {
    ERRORPRINT("Invalid arguments passed to SortList()\n");
    return bRetVal;
  }

  if (0 == plList->dwLength)
  {
    ERRORPRINT("Linked list is empty\n");
    return bRetVal;
  }

  pnStart = plList->pnHead;

  // Bubble sort
  for (DWORD i = 0; i < plList->dwLength; i++)
  {
    pnTmp = pnStart->pnNext;

    for (DWORD j = 0; j < plList->dwLength - i - 1; j++)
    {
      if ((*fptr)(pnStart->pvData, pnTmp->pvData)) // Compare data
      {
        // Swap data
        pvTmp           = pnStart->pvData;
        pnStart->pvData = pnTmp->pvData;
        pnTmp->pvData   = pvTmp;
      }
      pnTmp = pnTmp->pnNext;
    }
    pnStart = pnStart->pnNext;
  }

  bRetVal = TRUE;
  return bRetVal;
}

BOOL FindData(PLINKED_LIST plList, INT (*fptr)(PVOID, PVOID), PVOID pvValue)
{
  BOOL  bRetVal   = FALSE;
  PNODE pnCurrent = NULL;

  if (NULL == plList || NULL == fptr || NULL == pvValue)
  {
    ERRORPRINT("Invalid arguments passed to FindData()\n");
    return bRetVal;
  }

  if (0 == plList->dwLength)
  {
    ERRORPRINT("Linked list is empty\n");
    return bRetVal;
  }

  pnCurrent = plList->pnHead;

  for (DWORD i = 0; i < plList->dwLength; i++)
  {
    if ((*fptr)(pnCurrent->pvData, pvValue))
    {
      DEBUGPRINT("Found a match\n");
      bRetVal = TRUE;
      return bRetVal;
    }
    pnCurrent = pnCurrent->pnNext;
  }

  DEBUGPRINT("Did not find a match\n");
  return bRetVal;
}
