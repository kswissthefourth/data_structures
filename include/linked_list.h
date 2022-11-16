#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define LIST_SIZE sizeof(linkedlist_t)
#define NODE_SIZE sizeof(node_t)

#include <stdlib.h>
/**
 * @brief Node struct to be placed inside the linkedlist struct
 * 
 */
typedef struct node
{
    void *data;
    struct node *next;

}node_t;

/**
 * @brief linkedlist structure to hold the nodes
 * 
 */
typedef struct linkedlist
{
    int length;
    node_t *head;

}linkedlist_t;

/**
 * @brief Create a linked list object 
 * 
 * @return linkedlist_t* that contains no nodes and has a length of zero
 */
linkedlist_t *create_linked_list();

/**
 * @brief Create a node object
 * 
 * @param next node_t struct to be pointed to by newly created node
 * @param data data to be passed to the node
 * @param size size of the data type to be added
 * @return node_t* return the newly created node
 */
node_t *create_node(node_t *next, void *data, size_t size);

/**
 * @brief Get the node in the circularly linked list
 * 
 * @param list linkedlist struct 
 * @param index location of node that is trying to be returned
 * @return node_t* node returned based on the index given
 */
node_t *get_node(linkedlist_t *list, int index);

/**
 * @brief Add a new node to the end of the circularly linked list
 * 
 * @param list linkedlist struct 
 * @param data data to be passed to the node
 * @param size size of the data type to be added
 */
void add_end(linkedlist_t *list, void *data, size_t size);

/**
 * @brief Add a new node to the front of the circularly linked list
 * 
 * @param list linkedlist struct
 * @param data data to be passed to the node
 * @param size size of the data type to be added
 */
void add_front(linkedlist_t *list,void *data, size_t size);

/**
 * @brief Function to insert a node at any location in the circularly linked list
 * 
 * @param list linkedlist struct
 * @param index location of where the node is to be inserted after
 * @param data data to be added to the created node
 * @param size size of the data type to be added
 */
void insert_node(linkedlist_t *list, int index, void *data, size_t size);

/**
 * @brief Remove and free the node at the head of the linked list
 * 
 * @param list linkedlist struct 
 * @param fptr function pointer to a function to free specified data from node
 */
void remove_node(linkedlist_t *list, void (*fptr)(void *));

/**
 * @brief Remove specific node from a circularly linked list
 * 
 * @param list linkedlist struct
 * @param index index of node to be removed
 * @param fptr function pointer to a function to free specified data from node
 */
void remove_selected(linkedlist_t *list, int index, void (*fptr)(void *));

/**
 * @brief Free the list, its nodes, and data for each node from memory
 * 
 * @param list linkedlist struct
 * @param fptr function pointer to a function to free specified data from node
 */
void destroy_list(linkedlist_t *list, void (*fptr)(void *));

/**
 * @brief Traverse the circularly linked list and finish at the head
 * 
 * @param list linkedlist struct
 * @param fptr pointer to a function to print data in each node
 */
void traversal(linkedlist_t *list, void (*fptr)(void *));

/**
 * @brief Function to sort nodes based on their data using a bubble sort algorithm
 * 
 * @param list linkedlist struct
 * @param fptr Function pointer to compare function in sort.h
 */
void sort(linkedlist_t *list, int (*fptr)(void *, void *));

/**
 * @brief function to find the first occurrence of an item in a linked list
 * 
 * @param list linkedlist struct
 * @param fptr function pointer to check function in check.h
 * @param value void ptr to a data type to see if it is equal to the data at a given node
 */
void find_data(linkedlist_t *list, int (*fptr)(void *, void *), void *value);

#endif
