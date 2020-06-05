/*
 * File:	btree.c
 *
 * Implements a simple binary tree data structure.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

/*
 * create a new node with the provided data and return it
 *
 */
node_td *
BTreeNewNode(int key, node_td *parent, int index, void *data)
{
    node_td	*p;

    p = (node_td *) malloc(sizeof(node_td));

    p->key = key;
    p->index = index;
    p->count = 1;
    p->data = data;

    p->left = (node_td *) NULL;
    p->right = (node_td *) NULL;
    p->next = (node_td *) NULL;
    p->parent = parent;

    return p;
}

/*
 * is this node a leaf node?
 *
 * returns TRUE or FALSE
 */
int
BTreeNodeIsLeaf(node_td *p)
{
    if (p == (node_td *) NULL) {
#ifdef DEBUG
	fprintf(stderr,"ERROR : BTreeNodeIsLeaf(p) == NULL\n");
#endif
	return 1;
    }

    if (p->left == (node_td *) NULL && p->right == (node_td *) NULL)
        return 1;
    else
        return 0;
}

/*
 * insert a new node in the tree at the proper spot
 * based on the key value.
 *
 * key is the key value, (new node < key) goes to the left child, 
 * (new node > key) goes to the right child.
 *
 * (new node == key) is a special case, we chain a linked list of nodes
 * using the next pointer and increment the count field.
 *
 */
node_td *
BTreeInsertNode(node_td *root, int key, node_td *parent, int index, void *data)
{
    if (root == (node_td *) NULL) {
	return BTreeNewNode(key, parent, index, data);
    } else if (key < root->key) { /* add down left child sub-tree */ 
	root->left = BTreeInsertNode(root->left, key, root, (2*root->index)+1, data);
    } else if (key > root->key) { /* add down right child sub-tree */
	root->right = BTreeInsertNode(root->right, key, root, (2*root->index)+2, data);
    } else if (key == root->key) { /* duplicate key, add to the linked list pointed to by next */
	node_td	*p;

        root->count++;
	p = root;
	while (p->next != (node_td *) NULL) { 
	    p = p->next;
        }
	p->next = BTreeNewNode(key, parent, index, data);
    }

    return root;
}


/*
 * how deep is this tree?
 *
 * Returns the height of the tree (number of levels)
 */
int
BTreeGetHeight(node_td *node)
{
    int		leftMax, rightMax;

    if (node == NULL) {
#ifdef DEBUG
	fprintf(stderr,"ERROR : BTreeGetHeight(p) == NULL\n");
#endif
        return 0;
    }
    leftMax = 1 + BTreeGetHeight(node->left);
    rightMax = 1 + BTreeGetHeight(node->right);
    if (leftMax > rightMax) {
        return leftMax;
    } else {
        return rightMax;
    }
}


/*
 * Find a node in the tree, return it
 *
 * Efficiently traverse the tree looking for node with <key>
 *
 * If found, return it. If not found, return NULL
 */
node_td *
BTreeFindNode(node_td *root, int key)
{
    if (root == (node_td *) NULL) {	/* not found */
	return (node_td *) NULL;
    }

    if (key == root->key) {
        return root;
    }

    if (key < root->key) {
        return BTreeFindNode(root->left, key);	/* follow left subtree */
    } else {
        return BTreeFindNode(root->right, key);	/* follow right subtree */
    }
}


/*
 * remove a node from the tree
 *
 * UNIMPLEMENTED!
 *
 * (trickier than it sounds... )
 *
 */
node_td *
BTreeDeleteNode(node_td *root, int key)
{
#if 0 /* TODO */
    /*

    if root == null, return null

    if key > root->key
       root->right = delete_node(root->right, key)
    else if (key < root->key)
       root->left = delete_node(root->left, key)
    else if (key == root->key) {
	handle one child

        handle two child
    }

return root

    */
#endif
    return (node_td *) NULL;
}



