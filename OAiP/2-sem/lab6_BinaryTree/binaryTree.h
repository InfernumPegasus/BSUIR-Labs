//
// Created by USER on 27.05.2021.
//

#ifndef LAB6_BINARYTREE_BINARYTREE_H
#define LAB6_BINARYTREE_BINARYTREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRING_SIZE 30

struct info
{
    // keyword
    char origin[STRING_SIZE];
    char translation[STRING_SIZE];
};

struct treeNode
{
    // variable with information from struct "info"
    struct info record;

    // left and right pointers
    struct treeNode *left;
    struct treeNode *right;
};

/*------OUTPUT-------*/
void recursivePreOrder(struct treeNode *);
void showTreeUsingStack(struct treeNode *);
void printNode(struct treeNode *);
void outputMenu(struct treeNode *);

/*-------SEARCH-------*/
struct treeNode *search (struct treeNode * , char key[]);

/*-----BALANCING-----*/
struct treeNode *leftLeftRotation (struct treeNode *);
struct treeNode *rightRightRotation (struct treeNode *);
struct treeNode *leftRightRotation (struct treeNode *);
struct treeNode *rightLeftRotation (struct treeNode *);
struct treeNode *balance (struct treeNode *);

/*------DELETING-----*/
void deleteNodeByKey(struct treeNode ** , char Data[]);
void destroyTree (struct treeNode **);

/*-------INPUT-------*/
struct info getInfo();
struct treeNode *insert (struct treeNode * , struct info );

/*-------OTHER-------*/
int inputCheck();
int menu();

#endif //LAB6_BINARYTREE_BINARYTREE_H
