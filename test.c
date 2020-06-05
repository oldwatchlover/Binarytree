/*
 * File:	test.c
 *
 * Test program to exercise the binary tree data structure.
 *
 * Create a tree filled with random data, then print it out in various ways.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "btree.h"
#include "btree_util.h"

char    *ProgramName;

/* control strings for colored text output: */
#define DEFAULT_COLOR_TEXT      "\033[0m"       /* goes back to shell default color */
#define RED_COLOR_TEXT          "\033[1;31m"
#define GREEN_COLOR_TEXT        "\033[1;32m"


/* node keys (and node count) will range from 0 to MAX_KEY 
 * we use relatively small numbers (<32) so we can print out the tree nicely formatted
 * (it works just fine with larger values, but the output gets unwieldly)
 */
#define MAX_KEY (18)
static int	test_size = MAX_KEY;

static float
my_rand(void)
{
    float       val;

    val = (float)rand() / (float)RAND_MAX;
    return(val);
}


/*
 * main routine
 *
 */
int
main(int argc, char *argv[])
{
    int         i, key;
    node_td	*root;
    time_t	t;

    ProgramName = (char *) malloc(strlen(argv[0])+1);
    strcpy(ProgramName, argv[0]);

    srand((unsigned) time(&t)); /* seed rand() */

#define USAGE_STRING    "[-h] [-s size]"

    while ((argc > 1) && (argv[1][0] == '-')) {

        switch(argv[1][1]) {

	  case 'h':
	    fprintf(stderr,"%s : %s\n",ProgramName,USAGE_STRING);
	    exit(EXIT_SUCCESS);
	    break;

          case 's':
	    test_size = atoi(argv[2]);
	    argc--;
	    argv++;
            break;
   
          default:
            fprintf(stderr,"%s : %s : program option [%s] not recognized. (File %s, line %d)\n", 
		ProgramName, "WARNING", argv[1], __FILE__, __LINE__);
            fprintf(stderr,"%s : %s\n",ProgramName,USAGE_STRING);
            break;
        }
        argc--;
        argv++;
    }


    /*
     * build a binary tree.
     *
     * We'll start with a root node in the middle of our key value range, so it should
     * be a relatively balanced tree.
     *
     */
    root = BTreeNewNode(test_size/2, NULL, 0, NULL);

    for (i=1; i<test_size; i++) {
	key = (int) (my_rand() * (float)test_size);
	root = BTreeInsertNode(root, key, root, 0, NULL);
    }

    fprintf(stdout,"tree is %d levels high\n",BTreeGetHeight(root));

	/*
 	 * the first 6 levels of the tree will be printed out fancy
         * (sorry for the wide output) so you can trace the nodes
         * and their children visually
         */
    fprintf(stdout,"%s : Print by Level Traversal:\n",ProgramName);
    BTreeUtilPrintByLevel(root);
    fprintf(stdout,"\n");

	/* search our tree for some values, see if they are there.... */

    fprintf(stdout,"%s : test our tree by searching for some values...\n\n",ProgramName);

    for (i=0; i<test_size; i++) {
        node_td *p = BTreeFindNode(root, i);						
        if (p == (node_td *)NULL) {							
            fprintf(stdout,"%s : searching for node (%02d)... %s%s%s\n",		
		ProgramName, i, RED_COLOR_TEXT, "Not Found!", DEFAULT_COLOR_TEXT);	
        } else {									
            fprintf(stdout,"%s : searching for node (%02d)... %s%s%s ",			
		ProgramName, i, GREEN_COLOR_TEXT, "Found!", DEFAULT_COLOR_TEXT);	
	    fprintf(stdout,"\tkey = %02d, count = %d, index = %d\n",			
		p->key,p->count,p->index);						
        }										
    }
    fprintf(stdout,"\n");

    fprintf(stdout,"%s : Inorder Traversal:\n",ProgramName);
    BTreeUtilPrintByInorderTraversal(root);
    fprintf(stdout,"\n");
    fprintf(stdout,"\n");

    fprintf(stdout,"%s : Reverse Inorder Traversal:\n",ProgramName);
    BTreeUtilPrintByReverseInorderTraversal(root);
    fprintf(stdout,"\n");
    fprintf(stdout,"\n");

#if 0
    fprintf(stdout,"%s : Preorder Traversal:\n",ProgramName);
    BTreeUtilPrintByPreorderTraversal(root);
    fprintf(stdout,"\n");
    fprintf(stdout,"\n");

    fprintf(stdout,"%s : Postorder Traversal:\n",ProgramName);
    BTreeUtilPrintByPostorderTraversal(root);
    fprintf(stdout,"\n");
    fprintf(stdout,"\n");
#endif

    exit (EXIT_SUCCESS);


}
    

