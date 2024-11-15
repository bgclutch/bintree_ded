
#include "../bintree_headers/callgame.h"
#include <stdio.h>
#include <string.h>
#include "../bintree_headers/treeRW.h"
#include "../bintree_headers/dump.h"
#include "../../stack_ded/stack_headers/colorised_output.h"


int call_game(const char* command)
{
    Tree tree = tree_ctor();
    Dump_St General_Dump = {};

    if(strcmp(command, GAMEMODE) == 0)
    {
        gamestart(tree.root);
    }
    else if(strcmp(command, COMPAREMODE) == 0)
    {
        comparestart(tree.root);
    }
    else if(strcmp(command, GETDEFINE) == 0)
    {
        getdefine(tree.root);
    }
    else if(strcmp(command, INSTRUCTIONS) == 0)
    {
        print_akinator_instruction();
    }
    else
    {
        fprintf(stderr, RED_TEXT("WRONG INPUT!\n\n"));
        print_akinator_instruction();
        tree_dtor(&tree);
        return EXIT_FAILURE;
    }

    create_png(&General_Dump, tree.root);
    AKN_ERR(write_tree_to_file(tree.root, DATABASE), AKINATOR_FILE_ERROR);
    tree_dtor(&tree);
    create_html(General_Dump);

    return EXIT_SUCCESS;
}