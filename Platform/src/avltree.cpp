#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "avltree.h"


#define ASSERT assert

#ifdef AVLTREE_DEBUG
#define DEBUGCODE(code) \
code\
;
#else
#define DEBUGCODE(code) code;
#endif


#ifdef AVLTREE_DEBUG
#include <stdio.h>
#endif /* AVLTREE_DEBUG */


DEBUGCODE(int maxdepth = 0); 

static avlnode_t * avl_tree_follower(avlnode_t * node);
static avlnode_t * avl_subtree_first(avlnode_t * node);

static void     avl_init(avltree_t * tree, avlcompfn_t * comp);

static void     avl_left_rotate(avltree_t  * tree, avlnode_t * node);
static void     avl_right_rotate(avltree_t * tree, avlnode_t * node);

static void     avl_double_left_rotate(avltree_t  * tree, avlnode_t * node);
static void     avl_double_right_rotate(avltree_t * tree, avlnode_t * node);

static void     avl_remove_balance(avltree_t * tree, avlnode_t * node);
static void     avl_remove_node(avltree_t    * tree, avlnode_t * node);


/********************************************************************
 * 
 *  before free the node all user data must have been freed
 * 
 ********************************************************************/ 

static void avl_free_node(avltree_t * tree, avlnode_t * node)
{
    if ((NULL == tree) || (NULL == node)) 
    {
        return ;
    }

    tree->node_count--;
        
    free(node);
}

static avlnode_t * avl_alloc_node(avltree_t * tree)
{
    int         nodesize;
    avlnode_t * pnodeTemp;

    if (NULL == tree)
    {
        return NULL;
    }
    /*malloc memofy for node that need to add the length of key */
    nodesize = sizeof(avlnode_t);
        
    pnodeTemp = (avlnode_t *)malloc(nodesize);

    memset(pnodeTemp, 0, nodesize);

    if (NULL == pnodeTemp)
    {
    	DEBUGCODE(printf("\nAVL: line = %4d function = %s: allocation memory fails ", __LINE__, __FUNCTION__););
    	printf("\nAVLTree: line = %4d function = %s: allocation memory fails ", __LINE__, __FUNCTION__);
    }
    else
    {
    	tree->node_count++;
    }
    
    return pnodeTemp;
}

/********************************************************************
 * 
 * avl_init :    allocates and initiliases a new tree structure
 * comp     :    pointer to comparison function to use for this tree
 * returns  :    nothing
 * description : allocates space for the tree structure and
 * 				 NULLs the root node pointer and saves pointer to comparison
 * 	             function to tree structure
 * 
 ********************************************************************/ 
avltree_t * avl_new(avlcompfn_t * comp)
{
    avltree_t *tree;

    tree = (avltree_t *)malloc(sizeof(avltree_t));

    avl_init(tree, comp);

    return tree;

} /* avl_new */


/******************************************************************************* 
   avl_init :    initiliases a tree structure
   tree :        pointer to structure to initialise
   comp :        pointer to comparison function to use for this tree
   returns :     nothing
   description : NULLs the root node pointer and saves pointer to comparison
                 function to tree structure
********************************************************************************/
static void avl_init(avltree_t * tree, avlcompfn_t * comp)
{
    if (NULL == tree)
    {
        return ;
    }
    tree->root          = NULL;
    tree->comp          = comp;
    tree->node_count    = 0;
} /* avl_init */


void avl_free(avltree_t * tree)
{
	ASSERT(tree);
	ASSERT(!tree->root);
	ASSERT(!tree->node_count);
	
	if (avl_empty(tree) && !tree->node_count) 
	{
		free(tree);
	}
	else
	{
		DEBUGCODE(printf("\nAVLTREE: avltree still has nodes in the tree !! "););
	}
	
}

/*******************************************************************************
   avl_search : search for a key from a tree
   tree :       pointer to the tree to search from
   key :        the key pointer to search for
   returns :    on success : the data pointer associated with the key
                on failure : NULL
********************************************************************************/
void * avl_search(avltree_t * tree, void * key)
{
    register avlnode_t * node;
    register int       cmp;

    if (!tree || !(node = tree->root)) return NULL;

    while (node && (cmp = tree->comp(key, node->key))) 
    //while (node && (cmp = COMPARE(key, node->key))) 
    {
        DEBUGCODE(if (!(node->bal == 1 || node->bal == 0 || node->bal == -1)) printf("\nAVLTree: There is a node whose bal is incorrect grr \n"));
        node = cmp < 0 ? node->left : node->right;
    }
    
    if (node) 
    {
        return node->data;
    }
    
    return NULL;

} /* avl_search */


/******************************************************************************* 
   avl_smallest : search for a key from a tree
   tree :       pointer to the tree to search from
   returns :    on success : the data pointer associated with the smallest key
                on failure : NULL
********************************************************************************/
void * avl_smallest(avltree_t *tree)
{
    avlnode_t *node;

    if (!tree || !(node = tree->root)) 
    {
        return NULL;
    }

    while(node && node->left)
    {
        node = node->left;
    }

    if (node)
    {
        return node->data;
    }
    else 
    {
        return NULL;
    }

} /* avl_smallest */


/******************************************************************************* 
   avl_insert :  insert a key-data pair in tree
   tree :        pointer to tree to insert the pair to
   key :         the key pointer
   data :        the data pointer
   returns :     on success : NULL
                 on failure : the data pointer allready 
                 associated with same key
   description : searches the tree for the key, if found returns the data pointer
                 otherwise creates new node with given key and data pointers under
                 the node the search ended and balances the tree if needed
********************************************************************************/
void *avl_insert(avltree_t * tree, void * key, void * data)
{
    register avlnode_t  *node;
    register avlnode_t  *prevnode;
    register int       cmp = 0;

    DEBUGCODE(int max = 0;);

    if (NULL == tree)
    {
        return NULL;
    }
    /*if tree is null then add the first node*/
    if (!(node = tree->root)) 
    {
        node = avl_alloc_node(tree);
        tree->root = node;
        node->bal = 0;
        
        //memcpy(node->key, key, tree->ulkey_len);
        node->key  = key;
        
        node->data = data;
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;
        return NULL;
    }

    /*found the node*/
    prevnode = NULL;
    
    while(node && (cmp = tree->comp(key, node->key)))
    //while(node && (cmp = COMPARE(key, node->key))) 
    {
        prevnode = node;
        node = cmp < 0 ? node->left : node->right;
        DEBUGCODE(max++);
    }
    DEBUGCODE(maxdepth = max > maxdepth ? max : maxdepth);

    
    if (node) 
    {    
        return node->data;
    }

    node = avl_alloc_node(tree);
    node->bal = 0;
    
    //memcpy(node->key, key, tree->ulkey_len);
    node->key  = key;
    
    node->data = data; 
    node->parent = prevnode;
    node->left = NULL;
    node->right = NULL;    
    
    if (cmp < 0)
    {
        prevnode->left = node;
    }
    else
    {
        prevnode->right = node;
    }
   
    /* now key is added, then balancing */

    do 
    {
        if (prevnode->left == node)
        {
            prevnode->bal--;
        }
        else 
        {
            prevnode->bal++;
        }
        node = prevnode;
        prevnode = prevnode->parent;
    } 
    while(prevnode && (-1 == node->bal || node->bal == 1));

    if (-1 <= node->bal && node->bal <= 1) 
    {
        return NULL;
    }
    
    if (node->bal < 0) 
    {
        if (node->left->bal <= 0)
        {
            node->bal = 0; node->left->bal = 0;
            avl_right_rotate(tree, node);
        } 
        else 
        {
            if (!node->left->right->bal) 
            {
                node->bal = 0;
                node->left->bal = 0;
            }
            else 
            {
                node->bal = node->left->right->bal == -1 ? 1 : 0;
                node->left->bal = node->left->right->bal == -1 ? 0 : -1; 
            }
            node->left->right->bal = 0;
            avl_double_right_rotate(tree, node);
        }
    }
    else 
    {
        if (node->right->bal >= 0) 
        {
           node->bal = 0; node->right->bal = 0;
           avl_left_rotate(tree, node);
        } 
        else 
        {
            if (!node->right->left->bal) 
            {
                node->bal = 0;
                node->right->bal = 0;
            } 
            else 
            {
                node->bal = node->right->left->bal == -1 ? 0 : -1;
                node->right->bal = node->right->left->bal == -1 ? 1 : 0; 
            }
            node->right->left->bal = 0;
            avl_double_left_rotate(tree, node);
        }
    } 
    
    return NULL;

} /* avl_insert */


/******************************************************************************* 
   avl_right_rotate : rotates subtree to right
   tree :             tree the subtree belongs to
   node :             pointer to root of the subtree to rotate
   returns :          nothing
   desctiprion :

   Si  subtree i
   ni  node i

             node                   n1                  
              /\                   /  \      
             /  \                 /    \     
           n1   S3               S1   node   
           /\          ====>          /  \   
          /  \                       /    \  
         S1  S2                     S2    S3 
********************************************************************************/
static void avl_right_rotate(avltree_t *tree, avlnode_t *node)
{
    if (node->parent) 
    {
        if (node->parent->left == node) 
        {
            node->parent->left = node->left;
        } else 
        {
            node->parent->right = node->left;
        }
    }
    else 
    {
        tree->root = node->left;
    }
    
    node->left->parent = node->parent;
    node->parent = node->left;
    node->left = node->left->right;

    if (node->left) 
    {
        node->left->parent = node;
    }
    
    node->parent->right = node;
} /* avl_right_rotate */


/*******************************************************************************
   avl_left_rotate : rotates subtree to left
   tree :            tree the subtree belongs to
   node :            pointer to root of the subtree to rotate
   returns :         nothing
   desctiprion :

   Si  subtree i
   ni  node i

           node                        n1
           /  \                        /\
          /    \                      /  \
         S1    n1                  node  S3
              /  \      ====>       /\
             /    \                /  \
            S2    S3              S1  S2  
********************************************************************************/
static void avl_left_rotate(avltree_t * tree, avlnode_t * node)
{
    if (node->parent)
    {
        if (node->parent->left == node)
        {
            node->parent->left = node->right;
        }
        else
        {
            node->parent->right = node->right;
        }
    }
    else
    {
    	tree->root = node->right;
    }

    node->right->parent = node->parent;
    node->parent = node->right;
    node->right = node->right->left;
    
    if (node->right) node->right->parent = node;
    
    node->parent->left = node;

} /* avl_left_rotate */


/******************************************************************************* 
   avl_double_right_rotate : rotates subtree doubly to right
   tree :                    tree the subtree belongs to
   node :                    pointer to root of the subtree to rotate
   returns :                 nothing
   desctiprion :

   Si  subtree i
   ni  node i

           node                           n2
           /  \                           /\
          /    \                         /  \
         n1    S4                       n1  node
        /  \            ====>          /\     /\
       /    \                         /  \   /  \
      S1    n2                       S1  S2 S3  S4
            /\
           /  \
          S2  S3
********************************************************************************/
static void avl_double_right_rotate(avltree_t *tree, avlnode_t *node)
{

    avl_left_rotate(tree, node->left);
    avl_right_rotate(tree, node);

} /* avl_double_right_rotate */

/******************************************************************************* 
   avl_double_left_rotate : rotates subtree doubly to left
   tree :                   tree the subtree belongs to
   node :                   pointer to root of the subtree to rotate
   returns :                nothing
   desctiprion :

   Si  subtree i
   ni  node i

                        node                          n1
                        /  \                          /\
                       /    \                        /  \
                      S1    n2                    node   n1
                            /\                    /\     /\
                           /  \  ====>           /  \   /  \
                          n1  S4               S1   S2 S3  S4
                          /\          
                         /  \          
                        S2  S3
********************************************************************************/
static void avl_double_left_rotate(avltree_t *tree, avlnode_t *node)
{

    avl_right_rotate(tree, node->right);
    avl_left_rotate(tree, node);

} /* avl_double_left_rotate */



/******************************************************************************* 
   avl_remove :       removes a key-data pair from a tree
   tree :             tree where to remove them from
   key :              key of the key-data pair
   returns :          on success : the data pointer associated with the key
                      on failure : NULL
********************************************************************************/
void * avl_remove(avltree_t * tree, void * key)
{
    register avlnode_t  *node;
    register int       cmp;
    void     *retval;

    if (!(node = tree->root)) 
    {
        return NULL;
    }
    
    while(node && (cmp = tree->comp(key, node->key)))
    //while(node && (cmp = COMPARE(key, node->key)))
    {
        node = cmp < 0 ? node->left : node->right;
    }
    if (!node) return NULL;
    
    retval = node->data;

    avl_remove_node(tree, node);

    return retval;

} /* avl_remove */

/******************************************************************************* 
   avl_remove_node :     removes given node from given tree
   tree :                pointer to tree the node belongs to
   node :                pointer to the node to remove
   returns :             nothing
   description :         updates links so that node won't exists
                         in the tree anymore and balances the tree
********************************************************************************/
static void avl_remove_node(avltree_t *tree, avlnode_t *node)
{
    avlnode_t *tmp;
    avlnode_t save;

    if (node->left && node->right) 
    {
        
        /* no for statement */
        for (tmp = node->right; tmp->left; tmp = tmp->left);
        
        //memcpy(node->key, tmp->key, tree->ulkey_len);
        
        node->key  = tmp->key;
        node->data = tmp->data;
        
        if (node == tree->root) 
        {
            tree->root = tmp;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = tmp;
        }
        else
        {
            node->parent->right = tmp;
        }
        save = *node;
        *node = *tmp;
        *tmp = save;

        if (node->parent != node) 
        {
            tmp->left->parent = tmp;
            tmp->right->parent = tmp;

            if (node->right) 
            {
                node->right->parent = node;
            }
            if (tmp == node->parent->left) 
            {
                node->parent->left = node;
            }
            else 
            {
                node->parent->right = node;
            }
        } 
        else 
        {
            node->parent = tmp;
            if (tmp->right == tmp) 
            {
                tmp->right = node;
                tmp->left->parent = tmp;
            } 
            else 
            {
                tmp->left = node;
                tmp->right->parent = tmp;
            }
        }

    }

    if (node->left)
    {
        tmp = node->left;
    }
    else
    {
        tmp = node->right;
    }

    if (node == tree->root)
    {
        if ((tree->root = tmp))
        {
            tmp->parent = NULL;
        }
        avl_free_node(tree,node);
        return;
    } 

    if (node == node->parent->left)
    {
        node->parent->left = tmp;
        node->parent->bal++;
    } 
    else
    {
        node->parent->right = tmp;
        node->parent->bal--;
    }

    if (tmp) 
    {
        tmp->parent = node->parent;
    }

    tmp = node;
    node = node->parent;
    avl_free_node(tree,tmp);
    avl_remove_balance(tree, node);

} /* avl_remove_node */


/******************************************************************************* 
   avl_remove_balance : balances tree after removing one node 
   tree :               pointer to tree where node was removed
   node :               pointer to node which was removed nodes parent
                        this node's 'bal' member must be updated according
                        to which child the removed node was
   returns :            nothing
   description :        balancing is done making at most O(logn) tree rotations
                        so that every node's subtree vary at most by one in height
********************************************************************************/
static void avl_remove_balance(avltree_t *tree, avlnode_t *node)
{
    /* ready to balance */
    while (node && node->bal != -1 && node->bal != 1) 
    {
        if (node->bal == -2) 
        {
            switch(node->left->bal) 
            {
                case -1:
                    node->bal = 0; node->left->bal = 0;
                    avl_right_rotate(tree, node);
                    break;
                case 0:
                    node->bal = -1; node->left->bal = 1;
                    avl_right_rotate(tree, node);
                    break;
                case 1:
                    switch (node->left->right->bal) 
                    {
                        case -1: node->bal = 1; node->left->bal =  0; break;
                        case  0: node->bal = 0; node->left->bal =  0; break;
                        case  1: node->bal = 0; node->left->bal = -1; break;
                    }
                    node->left->right->bal = 0;
                    avl_double_right_rotate(tree, node);
                    break;
            }
            node = node->parent;
        }
        else if (node->bal == 2)
        {
            switch(node->right->bal) 
            {
                case 1:
                    node->bal = 0; 
                    node->right->bal = 0;
                    avl_left_rotate(tree, node);
                    break;
                case 0:
                    node->bal = 1; node->right->bal = -1;
                    avl_left_rotate(tree, node);
                    break;
                case -1:
                    switch (node->right->left->bal) 
                    {
                        case -1:
                            node->bal =  0; 
                            node->right->bal = 1; 
                            break;
                        case  0: 
                            node->bal =  0; 
                            node->right->bal = 0; 
                            break;
                        case  1: 
                            node->bal = -1; 
                            node->right->bal = 0; 
                            break;
                    }
                node->right->left->bal = 0;
                avl_double_left_rotate(tree, node);
                break;
            }
            node = node->parent;
        } 

        if (node->bal || !node->parent) 
        {
            break;
        }
        if (node->parent->left == node)
        {
            node->parent->bal++;
        }
        else 
        {
            node->parent->bal--;
        }
        node = node->parent;
    }

} /* avl_remove_balance */


#ifdef AVLTREE_DEBUG

static int node_num;

static int avl_node_height(avlnode_t * node)
{
	int left_height, right_height;
	
	if (!node) return 0;
	
	left_height  = avl_node_height(node->left);
	right_height = avl_node_height(node->right);
	
	return 1 + (left_height > right_height ? left_height : right_height);
}

/* recursive check function for debugging ..
 */
static int avl_rec_check_tree(avlnode_t * node)
{
    int left, right;
    
    if (!(node->bal == -1 || node->bal == 0 || node->bal == 1))
    {
    	printf("eek! balance of %d\n", node->bal);
    	exit(1);
    }

    node_num++;
    
    if (node->left)
    {
    	left = avl_rec_check_tree(node->left);
    }
    else
    {
    	left = 0;
    }

    if (node->right)
    {
    	right = avl_rec_check_tree(node->right);
    }
    else
    {
    	right = 0;
    }

    if (node->bal != right - left)
    {
        
    	printf("\nAVLTREE: ERROR!! bal : %d, key : %d\n", node->bal, (int) node->key);
    	exit(1);
    }

    return (left > right ? left : right) + 1;

} /* avl_rec_check_tree */


/* this goes trough the nodes 
   and checks that Avl-properties
   held in each node.
   the recursive function above does all
   the work ..
*/
int avl_check_tree(avltree_t *tree)
{   
    int tree_height;
    
    node_num = 0;
    
    if (!tree->root) return 0;
    
    tree_height = avl_rec_check_tree(tree->root);

    if (node_num != tree->node_count)
    {
        printf("\nAVLTREE: ERROR!! node number inconsistent!! \n");
        return -1;
    }
    
    return tree_height;
}
#endif /* AVLTREE_DEBUG */


/******************************************************************************* 
   avl_recurse_destroy : frees recursively all nodes in tree
   tree :                pointer to avltree_t
   datafree :            pointer to functions accepting two void
                         pointers as parameters
   returns :             nothing
   description :         walk trough the tree in inorder calling
                         datafree function for every node with key and
                         data pointers as parameters, then free the node
********************************************************************************/
static void avl_recurse_destroy(avltree_t * tree, avlnode_t * node, avl_iter_fn_t * datafree, void * arg)
{
    if (NULL == tree)
    {
        return;
    }
    if (!node) 
    {
        return;
    }
    avl_recurse_destroy(tree, node->left, datafree, arg);

    if (datafree)
    {
        datafree(node->key, node->data, arg);
    }

    avl_recurse_destroy(tree, node->right, datafree, arg);

    avl_free_node(tree, node);

} /* avl_recurse_destroy */


/******************************************************************************* 
   avl_destroy : destroys trees nodes leaving just empty tree
   tree :        pointer to avltree_t
   datafree :    pointer to function accepting two void pointers
                 as parameters
   returns :     nothing
   description : calls avl_recurse_destroy if tree is non-empty
                 and sets root to NULL i.e. makes the tree empty
********************************************************************************/
void avl_destroy(avltree_t * tree, avl_iter_fn_t * datafree, void * arg)
{
    if (NULL == tree)
    {
        return ;
    }
    avl_recurse_destroy(tree, tree->root, datafree, arg);

    tree->root = NULL;

} /* avl_destroy */


/*******************************************************************************
   avl_recurse_iterate : iterates recursively through the tree
   node :                pointer to a node 
   func :                pointer to function which is called for every node in tree
   arg :                 argument passed to func
   returns :             nothing
   description :         recurses through the tree 
********************************************************************************/ 
static void avl_recurse_iterate(avlnode_t * node, avl_iter_fn_t * func, void * arg)
{

    if (!node) return;

    if (node->left) avl_recurse_iterate(node->left, func, arg);
    
    func(node->key, node->data, arg);

    if (node->right) avl_recurse_iterate(node->right, func, arg);

} /* avl_recurse_iterate */



/*******************************************************************************
   avl_iterate : iterates through the tree
   tree :        pointer to avltree_t
   func :        pointer to function which is 
                 called for every node in tree
   returns :     nothing
   description : calls avl_recurse_iterate 
********************************************************************************/
void avl_iterate(avltree_t *tree, avl_iter_fn_t *func, void *arg)
{
    avl_recurse_iterate(tree->root, func, arg);
} /* avl_iterate */



/*******************************************************************************
   avl_iter :    iterates through nodes of given tree in inorder walk
   tree :        poiner to tree
   func :        function to call for each node with node's key and 
                 data pointer and arg as parameters
   returns :     nothing
   description : searches first node of tree and loops then searching 
                 allways the following node in tree calling func for 
                 every node
********************************************************************************/   
void avl_iter(avltree_t *tree, avl_iter_fn_t *func, void *arg)
{
    register avl_iter_flag_t    flags;
    register avlnode_t        * node;
    register avlnode_t        * next;

    if (!tree->root)
        return;

    node = avl_subtree_first(tree->root);

    while(node)
    {
        flags = func(node->key, node->data, arg);
        if (flags & AVL_ITER_REMOVE) 
        {
            next = avl_tree_follower(node);
            avl_remove_node(tree, node);
            node = next;
        }
        if (flags & AVL_ITER_STOP) return;

        if (!(flags & AVL_ITER_REMOVE)) node = avl_tree_follower(node);
    }

} /* avl_iter */



/* avl_tree_follower : returns pointer to the next node of given node in 
                       inorder walk order
   node :              pointer to tree node
   returns :           pointer to next node in tree or NULL if no next node exists
   description :       descends to right in tree if node has a right child
                       or follows parent links until not 'coming up' from right
*/
static avlnode_t * avl_tree_follower(avlnode_t * node)
{

    if (node->right) 
    {
        for (node = node->right; node->left; node = node->left);
    }
    else if (node->parent) 
    {
        while (node->parent && node == node->parent->right) node = node->parent;
        node = node->parent;
    } 
    else 
    {
        node = NULL;
    }

    return node;

} /* avl_tree_follower */


        
/* avl_subtree_first : returns pointer to the first (the leftmost) node of 
                       given subtree
   node :              pointer to root node of the subtree
   returns :           pointer to first node of tree rooted at node
   description :       follows links to left child while such exists
*/
static avlnode_t * avl_subtree_first(avlnode_t *node)
{

    while (node->left) node = node->left;

    return node;

} /* avl_subtree_first */
    

/* avl_empty :   returns true if tree is empty
   tree :        the tree to check
   returns :     nonzero if tree empty, zero if not
*/
int avl_empty(avltree_t * tree)
{
	DEBUGCODE(if (tree->root && !tree->node_count) printf("\nAVL: node number incorrect !!! "););
	DEBUGCODE(if (!tree->root && tree->node_count) printf("\nAVL: node number incorrect !!! "););
	
    return !tree->root;

} /* avl_empty */

signed long avl_node_num(avltree_t * tree)
{
	return tree->node_count;
}


/* end-of avltree_t_t.c */
