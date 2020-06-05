
#
# binary tree data structure and test program
#

OBJS =		btree.o 
TEST_OBJ =	test.o btree_util.o

TARGET = test

CC =	gcc
#CC =	cc

LINT_FLAGS =
LINT_FLAGS += -ansi -pedantic -w -Wall -Wextra -Wunused -Wundef
LINT_FLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations
LINT_FLAGS += -Wno-deprecated -Wuninitialized -Wtraditional

CFLAGS =	-O2 -Wall
#CFLAGS += -DVERBOSE

LDFLAGS =

LIBS =	-lm	

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(TARGET) 

$(TARGET):	$(OBJS) $(TEST_OBJ)
	$(CC) $(OBJS) $(TEST_OBJ) $(LDFLAGS) $(LIBS) -o $@

clean:
	/bin/rm -f $(TARGET) $(OBJS) $(TEST_OBJ) 


