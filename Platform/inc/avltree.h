/*************************************************************************	
* 
*       AVL TREE Hearder file: avltree.h
* 
* Author: Zhang Xiaolong (Charlie)
* 
* revison history:
*          (1) 2007.12.24:  remove key buffer from the node
*  
**************************************************************************/

#ifndef AVLTREE_H_20071206
#define AVLTREE_H_20071206

#define AVL_ITER_STOP         0x1
#define AVL_ITER_REMOVE       0x2

typedef unsigned int avl_iter_flag_t;                   /* return flag of iterator function */     

typedef avl_iter_flag_t avl_iter_fn_t(void * key, 
		                              void * data, 
		                              void * arg);      /* iterator function type */ 

typedef int avlcompfn_t(void * key1, void * key2);      /* compare function type */

struct avl_node 
{
	struct avl_node     * parent;
	struct avl_node     * left;
	struct avl_node     * right;
	
	void          * data;
	void          * key;
	signed   char   bal;       /* balance = (height of right subtree) - (height of left subtree ) */
};

typedef struct avl_node avlnode_t;

typedef struct 
{
	avlnode_t       * root;     /* pointer to root node of the tree */
	avlcompfn_t     * comp;     /* pointer to comparison function of the tree */
	
    signed long     node_count; /*the counter of node, it is used for debug purpose */
} avltree_t;

avltree_t * avl_new(avlcompfn_t * compfn);

void avl_free(avltree_t         * tree);

void * avl_insert(avltree_t     * tree, 
		          void          * key, 
		          void          * data);

void * avl_search(avltree_t     * tree, 
		          void          * key);

void * avl_remove(avltree_t     * tree, 
		          void          * key);

void * avl_smallest(avltree_t   * tree);


void avl_iterate(avltree_t * tree, avl_iter_fn_t * func,     void * arg);

void avl_destroy(avltree_t * tree, avl_iter_fn_t * datafree, void * arg);

int  avl_empty(avltree_t * tree);

void avl_iter(avltree_t  * tree, avl_iter_fn_t * func, void * arg);

signed long avl_node_num(avltree_t * tree);


#ifdef AVLTREE_DEBUG
int      avl_check_tree(avltree_t * tree);
#endif

#endif /* avltree_t_H */

/* end-of avltree_t.h */
