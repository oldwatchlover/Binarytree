/*
 * File:	pad.h
 *
 * A utility data structure to assist printing out level-by-level tree in a nice format
 *
 */
#ifndef __PAD_H__
#define __PAD_H__

/*
 * 6 high binary tree, with positions numbered as they would if stored in an array.
 * I counted the spaces to locate each node and filled the index_table, so we can calculate how many spaces to pad when doing line by line output.
 *
 *
                                                                                     (00)
                                         (01)                                                                                    (02)
                    (03)                                      (04)                                          (05)                                      (06)
         (07)                  (08)                  (09)                  (10)                  (11)                  (12)                  (13)                  (14)
   (15)       (16)       (17)       (18)       (19)       (20)       (21)       (22)       (23)       (24)       (25)       (26)       (27)       (28)       (29)       (30)
(31) (32)  (33) (34)  (35) (36)  (37) (38)  (39) (40)  (41) (42)  (43) (44)  (45) (46)  (47) (48)  (49) (50)  (51) (52)  (53) (54)  (55) (56)  (57) (58)  (59) (60)  (61) (62)

x64 nodes
x128 nodes
x256 nodes
x512 nodes
...
 *
 */

static int index_table[64] =
{
84,	/* 00, root */
41,	/* 01 */
131,
19,	/* 03 */
62,
112,
152,
9,	/* 07 */
31,
53,
75,
97,
119,
141,
163,
3,	/* 15 */
14,
25,
36,
47,
58,
69,
80,
91,
102,
113,
124,
135,
146,
157,
168,
0,	/* 31 */
5,
11,
16,
22,
27,
33,
38,
44,
49,
55,
60,
66,
71,
77,
82,
88,
93,
99,
104,
110,
115,
121,
126,
132,
137,
143,
148,
154,
159,
165,
170,
0,	/* 64 */
};

#endif /* __PAD_H__ */

