#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"


int main(void)
{

    Node* root = (Node*)calloc(sizeof(Node), 1);

    NodeElem_t elem = 0;

    printf("input your root number:");
    scanf("%d", &elem);

    elem_ctor(&root->data, sizeof(NodeElem_t));

    memcpy(root->data, &elem, sizeof(NodeElem_t));

    while(scanf("%d", &elem) && elem != -1945)
    {
        if(insert(root, elem) != NODE_IS_OKAY)
            return NODE_INSERT_ERR;
        printf("input number:");
    }

    tree_print(root);
    fprintf(stderr, "\n");

    tree_branch_dtor(root);


    return 0;
}