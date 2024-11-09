#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"


int main(void)
{

    Node* root = (Node*)calloc(sizeof(Node), 1);

    NodeElem_t elem = 10;

    elem_ctor(&root->data, sizeof(NodeElem_t));

    fprintf(stderr, "penis %p\n", root->data);

    memcpy(root->data, &elem, sizeof(NodeElem_t));

    tree_print(root);
    fprintf(stderr, "\n\n");

    elem = 4;

    if(insert(root, elem) != NODE_IS_OKAY)
    {
        return NODE_INSERT_ERR;
    }

    tree_print(root);
    fprintf(stderr, "\n\n");

    elem = 12;

    if(insert(root, elem) != NODE_IS_OKAY)
    {
        return NODE_INSERT_ERR;
    }

    tree_print(root);
    fprintf(stderr, "\n\n");

    elem = 1;

    if(insert(root, elem) != NODE_IS_OKAY)
    {
        return NODE_INSERT_ERR;
    }

    tree_print(root);
    fprintf(stderr, "\n\n");

    elem = -100;

    if(insert(root, elem) != NODE_IS_OKAY)
    {
        return NODE_INSERT_ERR;
    }

    tree_print(root);
    fprintf(stderr, "\n\n");

    tree_branch_dtor(root);


    return 0;
}