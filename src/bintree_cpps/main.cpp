#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"
#include "../bintree_headers/akinator.h"
#include "../bintree_headers/treeRW.h"
#include "../../stack_ded/stack_headers/stack.h"
#include "../../stack_ded/stack_headers/colorised_output.h"




int main(const int argc, const char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, RED_TEXT("WRONG INPUT!\n\n"));
        print_akinator_instruction();
        return EXIT_FAILURE;
    }

    Tree tree = {};
    read_tree_from_file(&tree);
    Dump_St General_Dump = {};

    if(strcmp(argv[1], GAMEMODE) == 0)
    {
        gamestart(tree.root);
        create_png(&General_Dump, tree.root);
    }
    else if(strcmp(argv[1], COMPAREMODE) == 0)
    {
        comparestart(tree.root);
        create_png(&General_Dump, tree.root);
    }
    else if(strcmp(argv[1], GETDEFINE) == 0)
    {
        getdefine(tree.root);
        create_png(&General_Dump, tree.root);
    }
    else if(strcmp(argv[1], INSTRUCTIONS) == 0)
    {
        print_akinator_instruction();
        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stderr, RED_TEXT("WRONG INPUT!\n\n"));
        print_akinator_instruction();
        return EXIT_FAILURE;
    }


    AKN_ERR(write_tree_to_file(tree.root, DATABASE), AKINATOR_FILE_ERROR);
    tree_branch_dtor(tree.root, tree.buffer, strlen(tree.buffer));
    free(tree.buffer);
    create_html(General_Dump);

    return EXIT_SUCCESS;
}