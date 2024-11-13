#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"
#include "../bintree_headers/akinator.h"
#include "../bintree_headers/treeRW.h"


int main(int argc, char** argv)
{
    if(argc < 2 || strcmp(argv[1], INSTRUCTIONS) == 0)
    {
        print_akinator_instruction();
        return 0;
    }

    Tree tree = {};
    read_tree_from_file(&tree);
    Dump_St General_Dump = {};

    if(strcmp(argv[1], GAMEMODE) == 0)
    {
        create_png(&General_Dump, tree.root);
        gamestart(tree.root);
        create_png(&General_Dump, tree.root);
    }
    // else if(strcmp(argv[2], COMPAREMODE) == 0)
    // {
    //     comparestart();
    // }
    // else if(strcmp(argv[2], GETDEFINE) == 0)
    // {
    //     getdefine();
    // }
    else
    {
        print_akinator_instruction();
        fprintf(stderr, "try again!\n");
        return 0;
    }








    AKN_ERR(write_tree_to_file(tree.root, DATABASE), AKINATOR_FILE_ERROR);
    free(tree.buffer);
    tree_branch_dtor(tree.root);
    create_html(General_Dump);

    return 0;
}