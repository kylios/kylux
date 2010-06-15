#ifndef BST_H
#define BST_H  

#include    "type.h"
#include "common.h"

struct bst_elem
{
    struct bst_elem* left;
    struct bst_elem* right;
    /* The parent is required for iterators and for red-black-ness. */
    struct bst_elem* parent;
};

#define BST_ENTRY(ELEM, TYPE, MEMBER)   \
    ((TYPE*) ((ELEM) - offset_of((TYPE), (MEMBER))))

/* The comparator function for the red-black tree.
 * When we perform insertions or lookups on the index, the function that
 * defines the operation for comparisons must comply to this function typedef.
 * */
typedef int bst_compare_func (const struct bst_elem*, 
        const struct bst_elem*, 
        const void* aux);
typedef void bst_dump_func (const struct bst_elem*);


struct bst
{
    /* the root node */
    struct bst_elem* root;
    
    /* the comparator function */
    bst_compare_func* comparator;
};





/* Initializes a new red black tree.
 * TREE must point to a valid block of memory
 * FUNC must be a comparator function used for comparing elements in the index
 * */
void bst_init (struct bst* tree, bst_compare_func* func);

/* Inserts the data pointed to by DATA into TREE
 * TREE must be a valid initialized tree
 * DATA must be a valid pointer
 * */
bool bst_insert (struct bst* tree, struct bst_elem* data);

/* Deletes the data pointed to by DATA from TREE
 * TREE must be a valid initialized tree
 * DATA must be a valid pointer
 * */
struct bst_elem* bst_delete (struct bst* tree, struct bst_elem* data);

/* Looks up a value from the tree.  If it finds a match, returns it, 
 * otherwise, returns NULL.
 * */
struct bst_elem* bst_find (struct bst* tree, 
        const struct bst_elem* elem);

/* Prints out the contents of TREE using FUNC to print the data 
 * (if supplied)
 * TREE must be a valid initialized tree
 * FUNC (optional) must be of type rb_tree_dump_func
 * */
void bst_dump (struct bst* tree, bst_dump_func* func);


/* BST iterator functions.  */
struct bst_iterator
{
    struct bst_elem* node;
    struct bst* tree;
};

struct bst_iterator* bst_get_iterator (struct bst*);
struct bst_iterator* bst_get_reverse_iterator (struct bst*);
struct bst_elem* bst_get (struct bst_iterator*);
void bst_itr_remove (struct bst_iterator*);
struct bst_elem* bst_next (struct bst_iterator*);
struct bst_elem* bst_prev (struct bst_iterator*);



#endif

