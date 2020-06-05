
Binary Tree data structure and some test utilities.

I pulled this binary tree code out of a larger project and added
some test code around it to make it testable/usable.

Lots of fun recursive code in here!

    README.md           - this file
    Makefile            - builds the binary tree object and the test objects
    btree.c             - main data structure functions
    btree.h             - node structure, comments, include to use btree.c
    test.c              - a main() driver test program. Creates a tree,
                          searches it, prints it out a few different ways
    btree_util.c        - test code specific utilities to traverse the tree
                          in several ways (and print out the node data)
    btree_util.h        - include file for btree_util.c
    pad.h               - a cryptic data structure to help me print out
                          a pretty tree (if height < 6 levels)


