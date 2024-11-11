#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"
#include "../bintree_headers/akinator.h"


int main(void)
{
    // Node* root = (Node*)calloc(sizeof(Node), 1);
    // root->parent = nullptr;

    // NodeElem_t elem = 0;

    // printf("input your root number:");
    // scanf("%s", &elem);

    // elem_ctor(&root->data, sizeof(NodeElem_t));

    // memcpy(root->data, &elem, sizeof(NodeElem_t));

    // printf("input number:");
    // while(scanf("%s", elem) && elem != "q")
    // {
    //     if(insert(root, elem) != NODE_IS_OKAY)
    //         return NODE_INSERT_ERR;
    //     printf("input number:");
    // }

    Tree tree = {};

    read_tree_from_file(&tree);

    fprintf(stderr, "\n\nmain tree:%p\n", tree.root);

    // fprintf(stderr, "\n\n%p %p\n\n", tree.root->left, tree.root->right);

    // tree_print(tree.root);

    Dump_St General_Dump = {};

    prepare_graphic_file(General_Dump);
    graphic_dump(tree.root, &General_Dump);
    close_graphic_dump(General_Dump);

    make_html_file(General_Dump.HTML_DUMP);
    dot_to_png(General_Dump.GRAPHIC_DUMP, &General_Dump);
    close_file_html(General_Dump.HTML_DUMP);


    fprintf(stderr, "\n");



    tree_branch_dtor(tree.root);
    free(tree.buffer);


    return 0;
}