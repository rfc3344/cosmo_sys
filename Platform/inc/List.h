
#ifndef LIST_H_20071207
#define LIST_H_20071207

#define LISTTRUE  1
#define LISTFALSE 0

struct node 
{
	struct node * succ;
	struct node * pred;
};

typedef struct node list_node_t;

/* list structure */

/* Field description:
 *
 * - head of list consists of succeeding node (head) and preceding node (tail)
 *   head has never any preceding nodes, so 'tail' is always NULL
 *
 * - tail consists of succeeding node (tail) and preceding node (tailpred)
 *   tail has never any succeeding nodes, so it 'tail' is always NULL
 *
 * Because tail is always NULL, the same pointer to node is at the same time
 * part of head node and tail node.
 *
 *   | HEAD | <- succ             | head     | = succ of head
 *   | NODE | <- pred = NULL
 *                            =>  | tail     | = pred of head AND succ of tail
 *   | TAIL | <- succ = NULL
 *   | NODE | <- pred             | tailpred | = pred of tail
 */

typedef struct
{
	list_node_t * head;         /* | HEAD | starts here                   */
	list_node_t * tail;         /* | NODE |          | TAIL | starts here */
	list_node_t * tailpred;     /*                   | NODE |             */
} list_t;

/* prototypes */

list_t * list_new();
void     list_destroy(list_t * list);

void list_init(list_t *list);
void list_init_node(list_node_t *node);
void list_add_head(list_t *list, list_node_t *node);
void list_add_tail(list_t *list, list_node_t *node);
void list_insert(list_t *list, list_node_t *node, list_node_t *listnode);


list_node_t * list_get_first(list_t *list);
list_node_t * list_get_first_node(list_node_t *node);
list_node_t * list_get_next(list_node_t *node);
list_node_t * list_remove_first(list_t *list);
list_node_t * list_remove_last(list_t *list);

void list_remove(list_node_t *node);

int list_is_empty(const list_t *list);

#endif /* LIST_H */
