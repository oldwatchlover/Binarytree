/*
 * File:	btree_util.c
 *
 * This file holds some utility functions to test the binary tree...
 *
 * Traversing the tree in several ways and printing out the nodes.
 *
 * We don't use the parent or data pointers in here.
 *
 */
#include <stdio.h>

#include "btree.h"
#include "btree_util.h"

extern char *ProgramName;


/*
 * print out this node:
 *
 */
static void
print_node(node_td *p)
{
    if (p == (node_td *) NULL) {
        fprintf(stderr,"%s : tried to print (NULL) node\n",ProgramName);
    } else {
#ifdef VERBOSE
        fprintf(stdout,"%08x, ",(unsigned int)p);
#endif
        fprintf(stdout,"(%d) ",p->key);
#ifdef VERBOSE
        fprintf(stdout,"<%d> ",p->index);
	if (BTreeNodeIsLeaf(p)) {
	    fprintf(stdout,"(leaf node)\n");
	} else {
	    fprintf(stdout,"\n");
	}
	fprintf(stdout,"\tleft = %08x\n\tright = %08x\n\tparent = %08x\n",
		(unsigned int)p->left,
		(unsigned int)p->right, (unsigned int)p->parent);
	fprintf(stdout,"\n");
#endif
    }
}

/* icky global used by the recursive code to print level by level 
 * across one output line 
 */
static int    xpos = 0;	

#ifndef VERBOSE
/* static int index_table[64]; */
#include "pad.h"

/* add spaces, padding when printing out a node on this level line */
static void
add_pad(int level, int index)
{
    int		i, padcnt;

    if (index > 63) {
	fprintf(stdout," ");
	xpos++;
	return;
    }

    padcnt = index_table[index] - xpos;

    for (i=0; i<padcnt; i++)
	fprintf(stdout," ");

    xpos += padcnt;
}
#endif

/* print out one level of the tree, there is some recursive voodoo here */
static void 
print_level(node_td *node, int level, char *s)
{

    if (node != (node_td *) NULL && level == 0) {
#ifdef VERBOSE
        fprintf(stdout,"%d [%d] (%s) ", node->key,node->index,s);
#else
	add_pad(level, node->index);
        fprintf(stdout,"(%02d)", node->key);
	xpos += 4;
#endif
    }   else if (node != (node_td *) NULL) {
        print_level(node->left, level - 1, "L");
        print_level(node->right, level - 1, "R");
    }
}



/* public tree traversal functions: */

/*
 * traverse by level, print out level by level 
 * 
 * The first 6 levels get printed out like a 
 * pretty tree in your algorithms text book, after that
 * we just print the level on one line, unable to line
 * it up to the parent nodes.
 *
 */
void 
BTreeUtilPrintByLevel(node_td *root)
{
    int i;
    int levelCount = BTreeGetHeight(root);
    for (i = 0; i < levelCount; i++)
    {
	xpos = 0;	/* when printing out each level, start at left margin */

        print_level(root, i, "root");
#ifndef VERBOSE
	fprintf(stdout,"\n");
	fprintf(stdout,"\n");
	fprintf(stdout,"\n");
#endif
    }
}

/*
 * visit root node, then left child, then right child
 */
void
BTreeUtilPrintByPreorderTraversal(node_td *root)
{
    if (root != (node_td *) NULL) {
	print_node(root);
	BTreeUtilPrintByPreorderTraversal(root->left);
	BTreeUtilPrintByPreorderTraversal(root->right);
    }
}

/*
 * visit left child, then right child, then root
 */
void
BTreeUtilPrintByPostorderTraversal(node_td *root)
{
    if (root != (node_td *) NULL) {
	BTreeUtilPrintByPostorderTraversal(root->left);
	BTreeUtilPrintByPostorderTraversal(root->right);
	print_node(root);
    }
}

/*
 * in-order traversal
 *
 * This will print out a BST in (increasing) sorted order
 *
 * visit left child, root node, then right child
 */
void
BTreeUtilPrintByInorderTraversal(node_td *root)
{
    if (root != (node_td *) NULL) {
	BTreeUtilPrintByInorderTraversal(root->left);
	print_node(root);
	BTreeUtilPrintByInorderTraversal(root->right);
    }
}

/*
 * reverse in-order traversal
 *
 * This will print out a BST in (decreasing) sorted order
 *
 * visit right child, root node, then left child
 */
void
BTreeUtilPrintByReverseInorderTraversal(node_td *root)
{
    if (root != (node_td *) NULL) {
	BTreeUtilPrintByReverseInorderTraversal(root->right);
	print_node(root);
	BTreeUtilPrintByReverseInorderTraversal(root->left);
    }
}


