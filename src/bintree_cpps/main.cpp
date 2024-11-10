#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"


int main(void)
{
    Node* root = (Node*)calloc(sizeof(Node), 1);
    root->rank = 0;
    root->parent = nullptr;

    NodeElem_t elem = 0;

    printf("input your root number:");
    scanf("%d", &elem);

    elem_ctor(&root->data, sizeof(NodeElem_t));

    memcpy(root->data, &elem, sizeof(NodeElem_t));

    printf("input number:");
    while(scanf("%d", &elem) && elem != -1945)
    {
        if(insert(root, elem) != NODE_IS_OKAY)
            return NODE_INSERT_ERR;
        printf("input number:");
    }

    Dump_St General_Dump = {};


    prepare_graphic_file(General_Dump);
    graphic_dump(root, &General_Dump);
    close_graphic_dump(General_Dump);

    make_html_file(General_Dump.HTML_DUMP);
    dot_to_png(General_Dump.GRAPHIC_DUMP, &General_Dump);
    close_file_html(General_Dump.HTML_DUMP);


    tree_print(root);
    fprintf(stderr, "\n");



    tree_branch_dtor(root);


    return 0;
}