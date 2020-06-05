/*
 * File:	btree.h
 *
 * A simple binary tree data structure. See comments and the code how the nodes are used.
 *
 */

#ifndef __BTREE_H__
#define __BTREE_H__

/*
 * binary tree node
 *
 */
typedef struct node_st
{
    int			key;		/* the sort value */
    int			index;		/* index if the tree were stored in an array (useful for level by level output) */
    int			count;		/* (count-1) copies of "key" nodes are chained with the next pointer */
    void		*data;		/* opaque data pointer to hold whatever you want */
    struct node_st	*left, *right;	/* left and right children */
    struct node_st	*next;		/* linked list of nodes with the same key value */
    struct node_st	*parent;	/* parent of this node (for advanced uses!) */
} node_td;

extern node_td	*BTreeNewNode(int key, node_td *parent, int index, void *data);
extern int	BTreeNodeIsLeaf(node_td *p);
extern node_td	*BTreeInsertNode(node_td *root, int key, node_td *parent, int index, void *data);
extern node_td	*BTreeDeleteNode(node_td *root, int key);
extern int	BTreeGetHeight(node_td *root);
extern node_td	*BTreeFindNode(node_td *root, int key);

#endif /* __BTREE_H__ */


