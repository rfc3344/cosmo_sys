
#include <stdlib.h>
#include <assert.h>


/* Uncommend the following if you want the list functions to verify that
 * a node isn't added twice or removed twice
 */

#include "List.h"

/* defines */

#define ASSERT assert

#define HEXFMT "0x%p"
#if 0
#define INTFMT "%ld"
#endif
#define INTFMT "%d"

/******************************************************************************
 *
 * WARNING & NOTE!
 *
 * This is a very fast and efficient list module. These functions are very
 * simple and fast, but they have no way to verify the sanity of the input
 * values. The use of this module is simple, but the user has to keep in mind
 * the following rules:
 *
 * - Every list structure must be initialized before use => list_init()
 * - Every node structure must be initialized (once) => list_init_node()
 * - Initializing an already used list or node structure is not allowed.
 * - Each node structure is used to link the structure onto the list.
 * - Don't add any node structure twice without removing it on the same list.
 * - Don't use the same node structure to add a structure already on another
 *   list to another.
 * - Don't remove a node from list twice
 * - Very low-level list implementation
 * - => Checking that parameters are valid is not possible
 * - => You must be careful when using the module
 * - Meant for low-level purpose
 * - => Impossible to fail if the parameters are valid
 * - Clearly invalid parameters will cause assertions to fail
 *
 *****************************************************************************/

/* Initialize a list structure
 *
 * INPUT VALUES:
 *
 * M01) all the list structures are handled the same way
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 *
 */


list_t * list_new()
{
    return (list_t *)malloc(sizeof(list_t));
}

void list_destroy(list_t * list)
{
    ASSERT(list != NULL);
    ASSERT(list->head == (list_node_t *) &list->tail);
    ASSERT(list->tail == NULL);
    ASSERT(list->tailpred == (list_node_t *) &list->head);
    
    if (list)
    {
        free(list);
    }
}

/**
 * list_init:
 * @list: 
 * 
 * Initialize a list structure
 */
void list_init(list_t * list)
{
	ASSERT(list != NULL);
	list->head = (list_node_t *) &list->tail;
	list->tail = NULL;
	list->tailpred = (list_node_t *) &list->head;
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
}

/* Initialize a node structure
 *
 * INPUT VALUES:
 *
 * M02) all the node structures are handled the same way
 *    ) ILLEGAL: node is a null pointer (no test case will be done)
 *
 */

/**
 * list_init_node:
 * @node: 
 * 
 * Initialize a node structure.
 */
void list_init_node(list_node_t *node)
{
	ASSERT(node != NULL);
	node->succ = NULL;
	node->pred = NULL;
}

/* Add a node to the beginning of the list
 *
 * INPUT VALUES:
 *
 * M03) List is empty
 * M04) List is not empty
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 *    ) ILLEGAL: node is a null pointer (no test case will be done)
 */

/**
 * list_add_head:
 * @list: 
 * @node: 
 * 
 * Add a node to the beginning of the list.
 */
void list_add_head(list_t * list, list_node_t * node)
{
	ASSERT(list != NULL);
	ASSERT(node != NULL);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	
	ASSERT(node->succ == NULL);
	ASSERT(node->pred == NULL);
	
	list_insert(list, node, NULL);
	
	ASSERT(list->head == node);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	ASSERT(node->succ != NULL);
	ASSERT(node->pred == (list_node_t *) &list->head);
}

/* Add a node to the end of the list
 *
 * INPUT VALUES:
 *
 * M05) List is empty
 * M06) List is not empty
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 *    ) ILLEGAL: node is a null pointer (no test case will be done)
 */

/**
 * list_add_tail:
 * @list: 
 * @node: 
 * 
 * Add a node to the end of the list.
 */
void list_add_tail(list_t *list, list_node_t *node)
{	
	ASSERT(list != NULL);
	ASSERT(node != NULL);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	
	ASSERT(node->succ == NULL);
	ASSERT(node->pred == NULL);
	
	list_insert(list, node, list->tailpred);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred == node);
	ASSERT(node->succ == (list_node_t *) &list->tail);
	ASSERT(node->pred != NULL);
}

/* Insert the node AFTER the specified node on the list
 *
 * INPUT VALUES:
 *
 * M07) Listnode is NULL, use list_add_head()
 * M08) Listnode is a valid node
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 *    ) ILLEGAL: node is a null pointer (no test case will be done)
 */

/**
 * list_insert:
 * @list: 
 * @node: 
 * @listnode: 
 * 
 * Insert the node AFTER the specified node on the list.
 */
void list_insert(list_t *list, list_node_t *node, list_node_t *listnode)
{	
	ASSERT(node != NULL);
	ASSERT(list != NULL);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	
	ASSERT(node->succ == NULL);
	ASSERT(node->pred == NULL);
	
	if (listnode == NULL) {
			
		node->succ = list->head;
		node->pred = (list_node_t *) &list->head;
		list->head = node;
		node->succ->pred = node;
		return;
	}
	ASSERT(listnode->succ != NULL);
	node->succ = listnode->succ;
	node->pred = listnode;
	listnode->succ = node;
	node->succ->pred = node;
	ASSERT(node->succ != NULL);
	ASSERT(node->pred == listnode);
	ASSERT(node->pred->succ == node);
	ASSERT(node->succ->pred == node);
}

/* Get the first node in the list, NULL if empty
 *
 * See list_remove_first() for more details about the return value.
 *
 * INPUT VALUES:
 *
 * M09) List is empty
 * M10) List is not empty
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 */

/**
 * list_get_first:
 * @list: 
 * 
 * Get the first node in the list.
 * 
 * Returns: The first node or NULL if empty. See list_remove_first()
 * for more details about the return value.
 */
list_node_t * list_get_first(list_t *list)
{
	list_node_t *first;
	ASSERT(list != NULL);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	first = list->head;
	if (first->succ == NULL) return NULL;
	return first;
}

/* Get the next node in the list, NULL if no more nodes
 *
 * See list_remove_first() for more details about the return value.
 *
 * INPUT VALUES:
 *
 * M11) No more nodes in the list
 * M12) There are still nodes in the list
 *    ) ILLEGAL: node is a null pointer (no test case will be done)
 */

/**
 * list_get_next:
 * @node: 
 * 
 * Get the next node in the list.
 * 
 * Returns: Next node, or NULL if no more nodes.
 * See list_remove_first() for more details about the
 * return value.
 */
list_node_t * list_get_next(list_node_t *node)
{
	list_node_t *next;
	ASSERT(node != NULL);
	next = node->succ;
	if (next->succ == NULL) return NULL;
	
	ASSERT(node->pred != NULL);
	
	return next;
}

/**
 * list_get_first_node:
 * @node: 
 * 
 * Get the first node in the list without the list structure argument 
 * 
 * Returns: 
 */
list_node_t * list_get_first_node(list_node_t * node)
{
	list_node_t * prev;
	ASSERT(node != NULL);
	prev = node;
	while (prev->pred != NULL) 
	{
		prev = prev->pred;
		
		ASSERT(prev->succ != NULL);
		
	}
	return prev->succ; /* modified by Zhang xiaolong from (return prev) to (return prev->succ)*/
}

/* Remove the first node from the list
 *
 * The same pointer that was used when the node was inserted into the list
 * is returned. Usually the node structure should be in the beginning of the
 * structure to be inserted:
 *
 * struct myown *own = (struct myown *) list_remove_first(list);
 *
 * If it is not in the beginning, you have to calculate the beginning of the
 * actual structure:
 *
 * list_node_t *n = list_remove_first(list);
 * if (node != NULL) {
 *         struct myown *own = (struct myown *)
 *                             (((char *) n) - offsetof(struct myown, node));
 * }
 *
 * If the list is empty, NULL is returned.
 *
 * INPUT VALUES:
 *
 * M13) List is empty
 * M14) List is not empty
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 */

list_node_t * list_remove_first(list_t *list)
{
	list_node_t *node;

	ASSERT(list != NULL);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	if (list_is_empty(list) == LISTTRUE) return NULL;
	node = list->head;
	ASSERT(node != NULL);
	ASSERT(node->succ != NULL);
	ASSERT(node->pred != NULL);
	list_remove(node);
	
	ASSERT(node->succ == NULL);
	ASSERT(node->pred == NULL);
	
	return node;
}

/* Remove the last node from the list
 *
 * See list_remove_first() for more details about the return value.
 *
 * INPUT VALUES:
 *
 * M15) List is empty
 * M16) List is not empty
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 */

list_node_t * list_remove_last(list_t * list)
{
	list_node_t * node;

	ASSERT(list != NULL);
	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	if (list_is_empty(list) == LISTTRUE) return NULL;
	node = list->tailpred;
	ASSERT(node->succ != NULL);
	ASSERT(node->pred != NULL);
	ASSERT(node != NULL);
	list_remove(node);
	
	ASSERT(node->succ == NULL);
	ASSERT(node->pred == NULL);
	
	return node;
}

/* Remove a node from the list
 *
 * INPUT VALUES:
 *
 * M17) The node is the first one in the list
 * M18) The node is in the middle of the list
 * M19) The node is the last one in the list
 *    ) ILLEGAL: node is a null pointer (no test case will be done)
 */

void list_remove(list_node_t *node)
{
	ASSERT(node != NULL);
	ASSERT(node->succ != NULL);
	ASSERT(node->pred != NULL);
	
	node->pred->succ = node->succ;
	node->succ->pred = node->pred;
	
	node->succ = NULL;
    node->pred = NULL;
}

/* Is the list empty?
 *
 * INPUT VALUES:
 *
 * M20) The list is empty
 * M21) The list is not empty
 *    ) ILLEGAL: list is a null pointer (no test case will be done)
 */

int list_is_empty(const list_t *list)
{
	list_node_t *node;

	ASSERT(list->head != NULL);
	ASSERT(list->tail == NULL);
	ASSERT(list->tailpred != NULL);
	
	if (list->tailpred != (list_node_t *) &list->head) 
	{
		node = list->head;
		ASSERT(node->succ != NULL);
		ASSERT(node->pred != NULL);
		
		return LISTFALSE;
	}
	return LISTTRUE;
}
