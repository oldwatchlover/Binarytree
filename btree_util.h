/*
 * File:	btree_util.h
 *
 * Incude file for btree_util.c
 *
 */
#ifndef __BTREE_UTIL_H__
#define __BTREE_UTIL_H__


extern void	BTreeUtilPrintByLevel(node_td *root);
extern void	BTreeUtilPrintByPreorderTraversal(node_td *root);
extern void	BTreeUtilPrintByPostorderTraversal(node_td *root);
extern void	BTreeUtilPrintByInorderTraversal(node_td *root);
extern void	BTreeUtilPrintByReverseInorderTraversal(node_td *root);

#endif /* __BTREE_UTIL_H__ */


