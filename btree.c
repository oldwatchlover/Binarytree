/*
 * File:	btree.c
 *
 * Implements a simple binary tree data structure
 * and common operations on the tree.
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
    p->data = data;

    p->left = (node_td *) NULL;
    p->right = (node_td *) NULL;
    p->parent = parent;

    return p;
}

/*
 * empty out and free a node's memory
 *
 */
void
BTreeFreeNode(node_td *node)
{
    if (node == (node_td *)NULL)
	return;

    free(node);
}

/*
 * disassemble and deallocate the memory of an entire tree
 *
 * returns root (which will be NULL)
 *
 */
node_td *
BTreeFreeTree(node_td *root)
{
    if (root == (node_td *)NULL)
	return (node_td *)NULL;

    root->left = BTreeFreeTree(root->left);
    root->right = BTreeFreeTree(root->right);
    BTreeFreeNode(root);
    root = (node_td *)NULL;
    return (root);
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
	return 1;	/* is NULL considered a leaf node? I think so */
    }

    if ((p->left == (node_td *) NULL) && (p->right == (node_td *) NULL))
        return 1;	/* both children empty */
    else
        return 0;	/* at least one child, not a leaf node */
}

/*
 * insert a new node in the tree at the proper spot
 * based on the key value.
 *
 * key is the key value, (new node < key) goes to the left child, 
 * (new node > key) goes to the right child.
 *
 * (new node == key) is a special case; we do not allow dupicates so
 * we just ignore it. 
 *
 * Follow the recursion and the index math here... the index of a node
 * will be the location in a 0 based array if they binary tree were stored
 * in an array. We will use this info as a position to print out the array
 * in a pretty tree format.
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
    } else if (key == root->key) { /* duplicate key, ignore */
	/* ignore */
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
 * Find a node in the tree by key value and return it
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

/* Temporary data structures and functions for some of the more complicated operations
 * (delete node, rebalance tree, etc.)
 */

/* A linked list of nodes to keep track of any nodes that get orphaned if we delete their parent. 
 * After the deletion, we'll retrive nodes off this list and re-insert them into the tree
 */

typedef struct nodelist_st
{
    node_td		*node;
    struct nodelist_st	*next;
} nodelist_td;

static nodelist_td	*tempNodeList = (nodelist_td *)NULL;
static int		tempListLength = 0;

/*
 * utility function.
 *
 * Traverses a disconnected subtree and inserts them
 * into our temporary linked list
 *
 * The traversal is "preorder" traversal
 */
static void
buildTempList(node_td *subtree)
{
    nodelist_td		*p;

    if (subtree != (node_td *)NULL) {

	if (tempNodeList == (nodelist_td *) NULL) {	/* first item in tempNodeList */
	    tempNodeList = (nodelist_td *) malloc(sizeof(nodelist_td));
	    tempNodeList->node = subtree;
	    tempNodeList->next = (nodelist_td *) NULL;
	    tempListLength = 1;
	} else {
	    p = tempNodeList;	/* append at the end of the list */
	    while (p->next != (nodelist_td *) NULL) { 
	        p = p->next;
            }
	    p->next = (nodelist_td *) malloc(sizeof(nodelist_td));
	    p->next->node = subtree;
	    p->next->next = (nodelist_td *) NULL;
	    tempListLength++;
	}
	buildTempList(subtree->left);
	buildTempList(subtree->right);
    }
}

#ifdef DEBUG
/*
 * a utility to follow and print out the temporary list, for debugging
 */
static void
printTempList(void)
{
    nodelist_td		*p;

    fprintf(stdout,"tempNodeList ");

    p = tempNodeList;
    while (p != (nodelist_td *) NULL) { 
	fprintf(stdout,"--> (%d) ",p->node->key);
        p = p->next;
    }
    fprintf(stdout,"\n");
}
#endif

/*
 * after we are done, we want to clean up the temporary list and it's memory
 */
static void
freeTempList(void)
{
    nodelist_td	*node, *tp;
    
    if (tempNodeList != (nodelist_td *) NULL) {
        node = tempNodeList->next;
        free(tempNodeList);
	tempNodeList = (nodelist_td *) NULL;

        while (node != (nodelist_td *) NULL) {
            tp = node->next;
	    free(node);
	    node = tp;
        }
    }
}

/*
 * process the temporary list and insert the nodes back into the tree.
 * Notice that the root is a **pointer, we have to handle the case that the
 * root node changes, so we need a pointer to it, not just it's value
 */
static void
addTempListToTree(node_td **root)
{
    nodelist_td	*node;

    node = tempNodeList;
    while (node != (nodelist_td *) NULL) {
        *root = BTreeInsertNode(*root, node->node->key, *root, 0, node->node->data);
        node = node->next;
    }
}

/*
 * remove a node from the tree
 *
 * (trickier than it sounds... )
 *
 * Notice that the root is a **pointer, we have to handle the case that the
 * root node changes, so we need a pointer to it, not just it's value
 */
int
BTreeDeleteNode(node_td **root, int key)
{
    node_td	*deleteme, *parent;
    node_td	*subtreeL, *subtreeR;

    if (*root == (node_td *) NULL)
	return 0;	/* empty tree, return false */

    deleteme = BTreeFindNode(*root, key);

    if (deleteme == (node_td *)NULL) {
	return 0;	/* node to delete not found, return false */
    }

    parent = deleteme->parent;
    subtreeL = deleteme->left;
    subtreeR = deleteme->right;

	    /* easiest case: node is a leaf, delete it */
    if (BTreeNodeIsLeaf(deleteme)) {

	if (parent == NULL) {	/* node was the root and a leaf, tree empty now */
            BTreeFreeNode(deleteme);
	    *root = (node_td *) NULL;	/* root == deleteme */
	    return 1;
	}

	    /* make the parent's child pointing to deleteme NULL */
        if (key > parent->key)
	    parent->right = (node_td *) NULL;

        if (key < parent->key)
	    parent->left = (node_td *) NULL;

        BTreeFreeNode(deleteme);

	    /* nothing else to do */

 	return 1;
    }

	/* hardest case: if the node to be deleted has children,
	 * we remove the node but must re-insert nodes of the 2 subtrees
   	 * we do this by traversing the subtrees, making a temporary linked list
         * of all nodes, then process that list to re-insert the nodes into the tree
	 */

	/* if the node to remove was the root, special case:
	 * rebuild the tree with an empty root
	 */
    if (parent == NULL) {	/* node was the root */
	
        BTreeFreeNode(deleteme);
	*root = (node_td *) NULL;
        tempNodeList = (nodelist_td *) NULL;

	    /* choose one child to be the new root: */

	if (subtreeL != (node_td *)NULL) { 

            *root = BTreeNewNode(subtreeL->key, NULL, 0, subtreeL->data);
            buildTempList(subtreeL->left);
            buildTempList(subtreeL->right);
            buildTempList(subtreeR);

        } else if (subtreeR != (node_td *)NULL) {

            *root = BTreeNewNode(subtreeR->key, NULL, 0, subtreeR->data);
            buildTempList(subtreeL);
            buildTempList(subtreeR->left);
            buildTempList(subtreeR->right);

        } else {
	    /* can't happen (was a leaf node, already handled that case) */
        }

        addTempListToTree(root);
 
        subtreeL = BTreeFreeTree(subtreeL);	/* must free old tree(s) after we add the nodes */
        subtreeR = BTreeFreeTree(subtreeR);

        freeTempList();
        tempNodeList = (nodelist_td *) NULL;

	return 1;
    }

	/* last case = node is not the root, but it has children */

	    /* make the parent's child pointing to deleteme NULL */
    if (key > parent->key)
	parent->right = (node_td *) NULL;

    if (key < parent->key)
	parent->left = (node_td *) NULL;

	/* build a temp list of subtree nodes, then re-add them to the tree */

    tempNodeList = (nodelist_td *) NULL;
    buildTempList(subtreeL);
    buildTempList(subtreeR);
    addTempListToTree(root);

    subtreeL = BTreeFreeTree(subtreeL);	/* must free old tree(s) after we add the nodes */
    subtreeL = BTreeFreeTree(subtreeR);
    BTreeFreeNode(deleteme);
    freeTempList();
    tempNodeList = (nodelist_td *) NULL;

    return 1;
}

/*
 *
 * Re-balance the tree. Find the median key and make it the root, then rebuild the tree
 *
 */
node_td *
BTreeRebalance(node_td *root)
{
    node_td		*newroot;
    nodelist_td		*p, *lastp, *mednode;
    int			i, medkey;


	/* flatten the tree into a linked list */
    tempNodeList = (nodelist_td *) NULL;
    buildTempList(root);

	/* find the median key in the list:
         * choose the node in  the middle of the list 
         */
    medkey = tempListLength/2;
    i = 0;

    p = tempNodeList;
    lastp = (nodelist_td *) NULL;
    while (p->next != (nodelist_td *) NULL && i<medkey) { 
	lastp = p;
        p = p->next;
	i++;
    }

    mednode = p;
    newroot = BTreeNewNode(mednode->node->key, NULL, 0, mednode->node->data);

    if (lastp != (nodelist_td *)NULL) {
        lastp->next = p->next;	/* remove p from list */
    } else { /* only one element in temp list */
        freeTempList();
        tempNodeList = (nodelist_td *) NULL;
    }

    addTempListToTree(&newroot);

    BTreeFreeTree(root);
    free(mednode);
    freeTempList();

    return newroot;
}

