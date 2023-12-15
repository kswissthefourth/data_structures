#include <Windows.h>
#include "../include/debug.h"
#include "../include/linked_list.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    plList->pnHead = NULL;

    return plList;
}

PNODE CreateNode(PNODE pnNext, PVOID pvData, SIZE_T stSize)
{
    PNODE pnNode = NULL;

    pnNode = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, NODE_SIZE);
    if (NULL == pnNode)
    {
        ERRORPRINT("Node was not allocated properly. Error: %s\n", strerror(errno));
        return pnNode;
    }
    
    pnNode->pvData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, stSize);
    if (NULL == pnNode->pvData)
    {
        ERRORPRINT("Node data was not allocated properly. Error: %s\n", strerror(errno));
        goto end;
    }

    memcpy(pnNode->pvData, pvData, stSize); // Copy data to the node

    pnNode->pnNext = pnNext; // Set the node the newly created node will point to

end:
    return pnNode;
}

PNODE GetNode(PLINKED_LIST plList, DWORD dwIndex);
{
    PNODE pnCurrent = NULL;

    // Check if a proper index exists to get the node
    if (dwIndex < 0 || dwIndex >= plList->dwLength || NULL == plList)
    {
        ERRORPRINT("Invalid arguments passed to GetNode()\n");
        goto end;
    }
    
    pnCurrent = plList->pnHead; // Set the current node to the head of the list
    
    // Loop through linked list to find node at given index
    for (DWORD i = 1; i <= dwIndex && pnCurrent != NULL; i++)
    {
        pnCurrent = pnCurrent->pnNext;
    }

end:
    return pnCurrent;
}

bool add_end(PLINKED_LIST plList, PVOID pvData, SIZE_T stSize)
{
    BOOL  bRetVal = FALSE;
    PNODE pnNode = NULL;
    PNODE pnPrev = NULL;

    if (NULL == plList || NULL == pvData)
    {
        ERRORPRINT("Invalid arguments passed to AddEnd()\n");
        goto end;
    }
    
    pnNode = CreateNode(plList->pnHead, pvData, stSize);
    if (NULL == pnNode)
    {
        ERRORPRINT("Failed to create node when adding to end of linked list\n");
        goto end;
    }

    if (plList->dwLength)
    {
        pnPrev = GetNode(plList, plList->dwLength - 1);
        if (NULL == pnPrev)
        {
            ERRORPRINT("Failed to get node\n");
            goto end;
        }
        pnPrev->pnNext = pnNode;
    }
    else
    {
        plList->pnHead = pnNode;
        pnNode->pnNext = plList->pnHead;
    }

    plList->dwLength++;
    bRetVal = TRUE;
end:
    return bRetVal;
}

BOOL AddFront(PLINKED_LIST plList, PVOID pvData, SIZE_T stSize)
{
    BOOL bRetVal = FALSE;
    PNODE *ppnHead = NULL;
    PNODE pnNode = NULL;

    if (NULL == plList || NULL == pvData)
    {
        ERRORPRINT("Invalid arguments passed to add_front()\n");
        return bRetVal;
    }

    PNODE *ppnHead = &plList->pnHead;

    PNODE pnNode = CreateNode(*ppnHead, pvData, stSize);
    if (NULL == pnNode)
    {
        ERRORPRINT("Failed to create node when adding to front of linked list\n");
        return bRetVal;
    }

    *ppnHead = pnNode;

    if (!plList->dwLength)
    {
        pnNode->pnNext = plList->pnHead;
    }

    plList->dwLength++;

    return bRetVal;
}

void insert_node(linkedlist_t *list, int index, void *data, size_t size)
{
    if (NULL == list || NULL == data)
    {
        fprintf(stderr, "Invalid parameters given to add_front()\n");
        return;
    }
    if (index < 0)
    {
        fprintf(stderr, "Index of %d is not a valid location for the list\n", index);
        return;
    }
    else if(index == list->length)
    {
        add_end(list, data, size);
    }
    else if (index > list->length)
    {
        add_end(list, data, size);
        
    }
    else if (index == 0)
    {
        add_front(list, data, size);
    }
    else
    {
        node_t *prev = get_node(list, index);

        prev->next = create_node(prev->next, data, size);

        list->length++;
    }
}

void remove_node(linkedlist_t *list, void (*fptr)(void *))
{

    if(NULL == list || !list->length) // Check if circularly linked list is empty or not
    {
        fprintf(stderr, "List is either empty or null\n");
        return;
    }

    node_t *next = NULL;
    node_t **head = &list->head;

    next = (*head)->next;
    if ((*head)->data != NULL)
    {
        (*fptr)((*head)->data); // Free the data from the head node
    }

    free(*head); // Free the head from memory
    (*head) = next; // Set the next node in the circularly linked list to the head

    
    list->length--; // Decrease the length of the circularly linked list

    // Check if only one item is left in the circularly linked list
    if (list->length > 0)
    {
        node_t *prev = get_node(list, list->length - 1); // get last node

        prev->next = list->head; // Set the last node to point to the head of the list
    }
}

void remove_selected(linkedlist_t *list, int index, void (*fptr)(void *))
{
    // Check if a valid index was passed
    if (index < 0 || index >= list->length)
    {
        ERRORPRINT("Invalid location in the linked list\n");
        exit(1);
    }

    // Check if there are any nodes in the list
    if (list->length == 0)
    {
        ERRORPRINT("Linked list currently does not have any nodes\n");
        exit(1);
    }

    node_t *current = list->head;
    node_t *tmp = NULL;

    for (size_t i = 0; i < index - 1; i++)
    {
        current = current->next; // Navigate to the node before the one to be removed
    }

    tmp = current->next; // Set tmp to the node that will be removed
    current->next = tmp->next; // Set current node to point the node that tmp points to

    // Check if data exists in node that will be removed
    if (tmp->data != NULL)
    {
        (*fptr)(tmp->data); // Free the data from the node
    }

    free(tmp); // Remove the node from the circularly linked list

    list->length--; // Decrease the length of the list by 1
}

void destroy_list(linkedlist_t *list, void (*fptr)(void *))
{
    if (list == NULL) // Check if the linked list is empty
    {
        return;
    }

    while(list->length > 0) // loop through the linked list
    {
        remove_node(list, fptr);
    }

    free(list); // Free the linkedlist struct from memory (destroy)
    
    DEBUGPRINT("Circularly linked list and its nodes have been destroyed (freed)\n");
}

void traversal(linkedlist_t *list, void (*fptr)(void *))
{

    node_t *current = list->head; // Get the head of the circularly linked list

    for (size_t i = 0; i <= list->length; i++)
    {
        (*fptr)(current->data); // print void data using specific print function
        current = current->next; // Move to the next node
    }
    printf("\n");
    DEBUGPRINT("End of circularly linked list traversal\n");

}

void sort(linkedlist_t *list, int (*fptr)(void *, void *))
{
    node_t *start = list->head;
    node_t *tmp = start->next;

    void *tmpdata = NULL;

    // Bubble sort
    for (size_t i = 0; i < list->length; i++)
    {
        for(size_t j = 0; j <= list->length; j++)
        {
            if((*fptr)(start->data, tmp->data)) // Compare data
            {
                // Swap data
                tmpdata = start->data;
                start->data = tmp->data;
                tmp->data = tmpdata;
                
            }
            tmp = tmp->next;
        }
        start = start->next; 
    }
}

void find_data(linkedlist_t *list, int (*fptr)(void *, void *), void *value)
{
    node_t *current = list->head; // Get the head of the circularly linked list
    for (size_t i = 0; i <= list->length; i++)
    {
        if (fptr(current->data,value))
        {
            DEBUGPRINT("Found a match\n");
            return;
        }
        current = current->next;
    }
    DEBUGPRINT("Did not find a match\n");
}
