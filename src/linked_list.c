#include "../include/debug.h"
#include "../include/linked_list.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

linkedlist_t *create_linked_list()
{
    linkedlist_t *list = NULL;
    if (NULL == (list = malloc(LIST_SIZE)))
    {
        ERRORPRINT("List was not allocated properly. Error: %s\n", strerror(errno));
        goto list_return;
    }
    list->length = 0; 
    list->head = NULL;

list_return:
    return list;
}

node_t *create_node(node_t *next, void *data, size_t size)
{
    node_t *node = NULL;

    // Allocate memory for the node
    if (NULL == (node = malloc(NODE_SIZE)))
    {
        ERRORPRINT("Node was not allocated properly. Error: %s\n", strerror(errno));
        goto end;
    }
    // Allocate memory for the node's data
    if (NULL == (node->data = malloc(size)))
    {
        ERRORPRINT("Node data was not allocated properly. Error: %s\n", strerror(errno));
        goto end;
    }

    memcpy(node->data, data, size); // Copy data to the node

    node->next = next; // Set the node the newly created node will point to

end:
    return node;
}

node_t *get_node(linkedlist_t *list, int index)
{
    node_t *current = NULL;
    // Check if a proper index exists to get the node
    if (index < 0 || index >= list->length) 
    {
        ERRORPRINT("Invalid arguments passed to get_node()\n");
        goto end;
    }
    
    current = list->head; // Set the current node to the head of the list
    
    // Loop through linked list to find node at given index
    for (size_t i = 1; i <= index && current != NULL; i++)
    {
        current = current->next;
    }

end:
    return current;
}

bool add_end(linkedlist_t *list, void *data, size_t size)
{
    bool retval = false;
    node_t *node = NULL;
    node_t *prev = NULL;

    if (NULL == list || NULL == data)
    {
        ERRORPRINT("Invalid arguments passed to add_end()\n");
        goto end;
    }
    
    node = create_node(list->head, data, size);
    if (NULL == node)
    {
        ERRORPRINT("failed to create node when adding to end of linked list\n");
        goto end;
    }

    if (list->length)
    {
        prev = get_node(list, list->length - 1);
        if (NULL == prev)
        {
            ERRORPRINT("Failed to get node\n");
            goto end;
        }
        prev->next = node;
    }
    else
    {
        list->head = node;
        node->next = list->head;
    }

    list->length++;
end:
    return retval;
}

bool add_front(linkedlist_t *list,void *data, size_t size)
{
    if (NULL == list || NULL == data)
    {
        ERRORPRINT("Invalid arguments passed to add_front()\n");
        return;
    }
    node_t **head = &list->head;

    node_t *node = create_node(*head, data, size);

    *head = node;

    if (!list->length)
    {
        node->next = list->head;
    }

    list->length++;
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
